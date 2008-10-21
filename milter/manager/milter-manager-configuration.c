/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2008  Kouhei Sutou <kou@cozmixng.org>
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef HAVE_CONFIG_H
#  include "../../config.h"
#endif /* HAVE_CONFIG_H */

#include "milter-manager-configuration.h"

#define MILTER_MANAGER_CONFIGURATION_GET_PRIVATE(obj)                   \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj),                                 \
                                 MILTER_MANAGER_TYPE_CONFIGURATION,     \
                                 MilterManagerConfigurationPrivate))

typedef struct _MilterManagerConfigurationPrivate MilterManagerConfigurationPrivate;
struct _MilterManagerConfigurationPrivate
{
    GList *child_milters;
    gboolean privilege;
    MilterStatus return_status;
    guint connection_timeout;
    guint sending_timeout;
    guint reading_timeout;
    guint end_of_message_timeout;
};

G_DEFINE_TYPE(MilterManagerConfiguration, milter_manager_configuration,
              G_TYPE_OBJECT);

static void dispose        (GObject         *object);
static void set_property   (GObject         *object,
                            guint            prop_id,
                            const GValue    *value,
                            GParamSpec      *pspec);
static void get_property   (GObject         *object,
                            guint            prop_id,
                            GValue          *value,
                            GParamSpec      *pspec);

static void
milter_manager_configuration_class_init (MilterManagerConfigurationClass *klass)
{
    GObjectClass *gobject_class;

    gobject_class = G_OBJECT_CLASS(klass);

    gobject_class->dispose      = dispose;
    gobject_class->set_property = set_property;
    gobject_class->get_property = get_property;

    g_type_class_add_private(gobject_class,
                             sizeof(MilterManagerConfigurationPrivate));
}

static void
milter_manager_configuration_init (MilterManagerConfiguration *configuration)
{
    MilterManagerConfigurationPrivate *priv;

    priv = MILTER_MANAGER_CONFIGURATION_GET_PRIVATE(configuration);
    priv->child_milters = NULL;
    priv->privilege = FALSE;
    priv->return_status = MILTER_STATUS_CONTINUE;
    priv->connection_timeout = 300; /* 5 minutes */
    priv->sending_timeout = 10; /* 10 seconds */
    priv->reading_timeout = 10; /* 10 seconds */
    priv->end_of_message_timeout = 300; /* 5 minutes */
}

static void
dispose (GObject *object)
{
    MilterManagerConfigurationPrivate *priv;

    priv = MILTER_MANAGER_CONFIGURATION_GET_PRIVATE(object);

    if (priv->child_milters) {
        g_list_foreach(priv->child_milters, (GFunc)g_object_unref, NULL);
        g_list_free(priv->child_milters);
        priv->child_milters = NULL;
    }

    G_OBJECT_CLASS(milter_manager_configuration_parent_class)->dispose(object);
}

static void
set_property (GObject      *object,
              guint         prop_id,
              const GValue *value,
              GParamSpec   *pspec)
{
    MilterManagerConfigurationPrivate *priv;

    priv = MILTER_MANAGER_CONFIGURATION_GET_PRIVATE(object);
    switch (prop_id) {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void
get_property (GObject    *object,
              guint       prop_id,
              GValue     *value,
              GParamSpec *pspec)
{
    MilterManagerConfigurationPrivate *priv;

    priv = MILTER_MANAGER_CONFIGURATION_GET_PRIVATE(object);
    switch (prop_id) {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

MilterManagerConfiguration *
milter_manager_configuration_new (void)
{
    return g_object_new(MILTER_MANAGER_TYPE_CONFIGURATION,
                        NULL);
}

gboolean
milter_manager_configuration_is_privilege_mode (MilterManagerConfiguration *configuration)
{
    return MILTER_MANAGER_CONFIGURATION_GET_PRIVATE(configuration)->privilege;
}

void
milter_manager_configuration_add_child_milter (MilterManagerConfiguration *configuration,
                                               MilterManagerChildMilter   *milter)
{
    MilterManagerConfigurationPrivate *priv;

    priv = MILTER_MANAGER_CONFIGURATION_GET_PRIVATE(configuration);

    g_object_ref(milter);
    priv->child_milters = g_list_append(priv->child_milters, milter);
}

const GList *
milter_manager_configuration_get_child_milters (MilterManagerConfiguration *configuration)
{
    MilterManagerConfigurationPrivate *priv;

    priv = MILTER_MANAGER_CONFIGURATION_GET_PRIVATE(configuration);
    return priv->child_milters;
}

MilterStatus
milter_manager_configuration_get_return_status_if_filter_unavailable
                                     (MilterManagerConfiguration *configuration)
{
    return MILTER_MANAGER_CONFIGURATION_GET_PRIVATE(configuration)->return_status;
}

guint
milter_manager_configuration_get_connection_timeout (MilterManagerConfiguration *configuration)
{
    return MILTER_MANAGER_CONFIGURATION_GET_PRIVATE(configuration)->connection_timeout;
}

guint
milter_manager_configuration_get_sending_timeout (MilterManagerConfiguration *configuration)
{
    return MILTER_MANAGER_CONFIGURATION_GET_PRIVATE(configuration)->sending_timeout;
}

guint
milter_manager_configuration_get_reading_timeout (MilterManagerConfiguration *configuration)
{
    return MILTER_MANAGER_CONFIGURATION_GET_PRIVATE(configuration)->reading_timeout;
}

guint
milter_manager_configuration_get_end_of_message_timeout (MilterManagerConfiguration *configuration)
{
    return MILTER_MANAGER_CONFIGURATION_GET_PRIVATE(configuration)->end_of_message_timeout;
}

/*
vi:ts=4:nowrap:ai:expandtab:sw=4
*/
