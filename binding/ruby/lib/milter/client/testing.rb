module Milter
  class Client
    module Test
      class MilterRunner
        def initialize(milter_path, *args)
          @milter_path = milter_path
          @args = args
          if @args.last.is_a?(Hash)
            @options = @args.pop
          else
            @options = {}
          end

          if @options[:env]
            @env = @options.delete(:env)
          else
            @env = {}
          end

          @timeout = @options.delete(:timeout) || 5

          @port = @options.delete(:port) || 20025
          @host = @options.delete(:host) || "localhost"
          @spec = "inet:#{@port}@#{@host}"
        end

        def run
          @pid = spawn(@env,
                       RbConfig.ruby,
                       @milter_path,
                       "--connection-spec=#{@spec}",
                       *@args,
                       @options)

          start = Time.now
          Process.detach(@pid)
          loop do
            break if Time.now - start > @timeout
            begin
              TCPSocket.new(@host, @port)
              break
            rescue SystemCallError
            end
          end
          @pid
        end

        def stop
          Process.kill(:KILL, @pid)
        rescue SystemCallError
        end
      end
    end
  end
end
