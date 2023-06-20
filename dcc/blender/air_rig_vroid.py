import typing
import bpy
from mathutils import Vector

class AirRigVroid(bpy.types.Operator):
    bl_idname = "air.rig_vroid"
    bl_label = "Rig Vroid"
    
    def execute(self, context):

        selected_objects = bpy.context.selected_objects
        selected_armature = None
        for obj in selected_objects:
            if obj.type == 'ARMATURE':
                selected_armature = obj
                break

        if selected_armature:
            print("Selected Armature", selected_armature.name)

            bpy.context.view_layer.objects.active = selected_armature
            bpy.ops.object.mode_set(mode='EDIT')
            armature = selected_armature.data
            root = armature.edit_bones.get("Root")
            arm_r = armature.edit_bones.get("J_Bip_R_LowerArm")
            upper_arm_r = armature.edit_bones.get("J_Bip_R_UpperArm")
            pos_arm_r = selected_armature.pose.bones.get("J_Bip_R_LowerArm")
            pos_upper_arm_r = selected_armature.pose.bones.get("J_Bip_R_UpperArm")
            if arm_r and upper_arm_r:
                print("Find J_Bip_R_LowerArm")

                arm_r.head += Vector((0, -0.001, 0))
                upper_arm_r.tail += Vector((0, -0.001, 0))

                ik_arm_r = armature.edit_bones.new("IK_Arm_R")
                ik_arm_r.head = arm_r.tail
                ik_arm_r.tail = arm_r.tail + Vector((0, -0.2, 0))
                ik_arm_r.parent = root

                ikt_arm_r = armature.edit_bones.new("IKT_Arm_R")
                ikt_arm_r.head = pos_upper_arm_r.tail + Vector((0, -0.5, 0))
                ikt_arm_r.tail = pos_upper_arm_r.tail + Vector((0, -0.7, 0))
                ikt_arm_r.parent = root

                ik_arm_r_constraint = pos_arm_r.constraints.new(type='IK')
                ik_arm_r_constraint.target = selected_armature
                ik_arm_r_constraint.subtarget = ik_arm_r.name
                ik_arm_r_constraint.pole_target = selected_armature
                ik_arm_r_constraint.pole_subtarget = ikt_arm_r.name
                ik_arm_r_constraint.chain_count = 2
                ik_arm_r_constraint.use_tail = True
                ik_arm_r_constraint.use_stretch = True
                ik_arm_r_constraint.weight = 1.0
                ik_arm_r_constraint.use_location = True
                ik_arm_r_constraint.use_rotation = False
                ik_arm_r_constraint.influence = 1.0

            else:
                print("Not find J_Bip_R_LowerArm")
            # arm_l = armature.edit_bones.get("J_Bip_L_LowerArm")
            # pos_arm_l = selected_armature.pose.bones.get("J_Bip_L_LowerArm")
            # if arm_l:
            #     print("Find J_Bip_L_LowerArm")
            #     ik_arm_l = armature.edit_bones.new("IK_Arm_L")
            #     ik_arm_l.head = arm_l.tail
            #     ik_arm_l.tail = arm_l.tail + Vector((0, -0.2, 0))
            #     ik_arm_l.parent = root
            #     ik_arm_l_constraint = pos_arm_l.constraints.new(type='IK')
            #     ik_arm_l_constraint.target = selected_armature
            #     ik_arm_l_constraint.subtarget = ik_arm_l.name
            #     ik_arm_l_constraint.chain_count = 2
            # else:
            #     print("Not find J_Bip_L_LowerArm")
            
            bpy.ops.object.mode_set(mode='OBJECT')
        else:
            print("No armature selected.")

        bpy.context.view_layer.update()
        
        return {'FINISHED'}
