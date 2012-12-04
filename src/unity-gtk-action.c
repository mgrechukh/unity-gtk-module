#include "unity-gtk-action.h"

G_DEFINE_TYPE (UnityGtkAction,
               unity_gtk_action,
               G_TYPE_OBJECT);

enum
{
  ACTION_PROP_0,
  ACTION_PROP_NAME,
  ACTION_PROP_ITEM,
  ACTION_N_PROPERTIES
};

static GParamSpec *action_properties[ACTION_N_PROPERTIES] = { NULL };

static void
unity_gtk_action_set_name (UnityGtkAction *action,
                           const gchar    *name)
{
  g_return_if_fail (UNITY_GTK_IS_ACTION (action));

  g_free (action->name);
  action->name = g_strdup (name);
}

static void
unity_gtk_action_set_item (UnityGtkAction   *action,
                           UnityGtkMenuItem *item)
{
  UnityGtkMenuItem *old_item;

  g_return_if_fail (UNITY_GTK_IS_ACTION (action));

  old_item = action->item;

  if (item != old_item)
    {
      if (old_item != NULL)
        {
          action->item = NULL;
          g_object_unref (old_item);
        }

      if (item != NULL)
        action->item = g_object_ref (item);
    }
}

static void
unity_gtk_action_dispose (GObject *object)
{
  UnityGtkAction *action;
  GHashTable *items_by_name;

  g_return_if_fail (UNITY_GTK_IS_ACTION (object));

  action = UNITY_GTK_ACTION (object);
  items_by_name = action->items_by_name;

  if (items_by_name != NULL)
    {
      action->items_by_name = NULL;
      g_hash_table_unref (items_by_name);
    }

  unity_gtk_action_set_item (action, NULL);
  unity_gtk_action_set_name (action, NULL);

  G_OBJECT_CLASS (unity_gtk_action_parent_class)->dispose (object);
}

static void
unity_gtk_action_get_property (GObject    *object,
                               guint       property_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
  UnityGtkAction *self;

  g_return_if_fail (UNITY_GTK_IS_ACTION (object));

  self = UNITY_GTK_ACTION (object);

  switch (property_id)
    {
    case ACTION_PROP_NAME:
      g_value_set_string (value, self->name);
      break;

    case ACTION_PROP_ITEM:
      g_value_set_object (value, self->item);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
unity_gtk_action_set_property (GObject      *object,
                               guint         property_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
  UnityGtkAction *self;

  g_return_if_fail (UNITY_GTK_IS_ACTION (object));

  self = UNITY_GTK_ACTION (object);

  switch (property_id)
    {
    case ACTION_PROP_NAME:
      unity_gtk_action_set_name (self, g_value_get_string (value));
      break;

    case ACTION_PROP_ITEM:
      unity_gtk_action_set_item (self, g_value_get_object (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
unity_gtk_action_class_init (UnityGtkActionClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = unity_gtk_action_dispose;
  object_class->get_property = unity_gtk_action_get_property;
  object_class->set_property = unity_gtk_action_set_property;

  action_properties[ACTION_PROP_NAME] = g_param_spec_string ("name",
                                                             "Name",
                                                             "Name",
                                                             NULL,
                                                             G_PARAM_READWRITE);
  action_properties[ACTION_PROP_ITEM] = g_param_spec_object ("item",
                                                             "Item",
                                                             "Item",
                                                             UNITY_GTK_TYPE_MENU_ITEM,
                                                             G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, ACTION_N_PROPERTIES, action_properties);
}

static void
unity_gtk_action_init (UnityGtkAction *self)
{
}