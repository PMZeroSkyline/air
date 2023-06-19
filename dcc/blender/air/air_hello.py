import bpy

class AirHelloOperator(bpy.types.Operator):
    bl_idname = "air.hello"
    bl_label = "Hello"

    def execute(self, context):
        print("hello world")
        return {'FINISHED'}