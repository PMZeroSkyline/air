import bpy

class AirPanel(bpy.types.Panel):
    bl_idname = "OBJECT_PT_air_panel"
    bl_label = "Air Panel"
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    bl_category = "Air"

    def draw(self, context):
        layout = self.layout
        layout.label(text="Welcome Air Addon")
        layout.operator("air.hello")
        layout.operator("air.rig_vroid")
