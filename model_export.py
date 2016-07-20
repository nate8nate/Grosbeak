import bpy
import os.path
from bpy_extras.io_utils import ExportHelper

class ExportScene(bpy.types.Operator, ExportHelper):
    """Exports scene to a folder"""
    bl_idname = "export_scene.scene"
    bl_label = "Export Scene"

    filename_ext = "."

    def execute(self, context):
        path = os.path.dirname(self.filepath)

        objects = list(bpy.data.objects)
        for o in objects:
            print('\n')
            if o.type == 'MESH':
                mesh = o.to_mesh(bpy.context.scene, True, 'PREVIEW')
                o.data.calc_tessface()
                f = open(path + '/meshes/' + o.name + '.mesh', 'w')
                
                colors = [None] * len(mesh.vertices)*3
                for group in mesh.vertex_colors:
                    for k,v in enumerate(mesh.tessfaces):
                        colors[v.vertices[0]] = group.data[k].color
                        colors[v.vertices[1]] = group.data[k].color
                        colors[v.vertices[2]] = group.data[k].color
                
                f.write('%d ' % len(mesh.vertices))
                for v in mesh.vertices:
                    f.write('%f %f %f ' % (v.co.x, v.co.z, v.co.y))
                    f.write('%f %f %f ' % (v.normal.x, v.normal.z, v.normal.y))
                f.write('\n')
                
                f.write('%d ' % (3*len(mesh.tessfaces)))
                for i in mesh.tessfaces:
                    f.write('%d %d %d ' % (i.vertices[0], i.vertices[2], i.vertices[1]))
                f.write('\n')
                
                #For puzzle connections
                #for i in o.data.tessfaces:
                #    f.write('%d %d %d ' % (i.vertices[0], i.vertices[2], i.vertices[1]))
                #f.write('\n')
                
                
                for c in colors:
                    print('%f %f %f ' % (c[0], c[1], c[2]))
                
                bpy.data.meshes.remove(mesh)
                f.close
    #            elif o.type == 'LAMP': 

        return {'FINISHED'}

def menu_func_export(self, context):
    self.layout.operator(ExportScene.bl_idname, text="Scene")


def register():
    bpy.utils.register_class(ExportScene)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportScene)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()

    # test call
    bpy.ops.export_scene.scene('INVOKE_DEFAULT')
