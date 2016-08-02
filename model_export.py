import bpy
import os.path
from bpy_extras.io_utils import ExportHelper

class ExportScene(bpy.types.Operator, ExportHelper):
    """Exports scene to a folder"""
    bl_idname = "export_scene.scene"
    bl_label = "Export Scene"

    filename_ext = "."

    def execute(self, context):
        print('---------------------')
        path = os.path.dirname(self.filepath)

        for s in bpy.data.scenes:
            meshlist = []
            entitylist = []
            pointlist = []
            dirlist = []

            scenefile = open(path + '/scenes/' + s.name + '.scene', 'w')

            for o in s.objects:
                if o.type == 'EMPTY' and o.dupli_group:
                    for dgo in o.dupli_group.objects:
                        if dgo.type == 'MESH':
                            meshlist.append(dgo.name)
                            mesh = dgo.to_mesh(bpy.context.scene, True, 'PREVIEW')
                            dgo.data.calc_tessface()
                            meshfile = open(path + '/meshes/' + dgo.name + '.mesh', 'w')
                            hasVertexColors = len(mesh.vertex_colors) > 0
                            if hasVertexColors:
                                colors = [None] * len(mesh.vertices)
                                for group in mesh.vertex_colors:
                                    for k,v in enumerate(mesh.tessfaces):
                                        index0 = v.vertices[0]
                                        index1 = v.vertices[1]
                                        index2 = v.vertices[2]
                                        colors[index0] = group.data[k*3].color
                                        colors[index1] = group.data[k*3+1].color
                                        colors[index2] = group.data[k*3+2].color
                            else:
                                colors = []
                                for _ in mesh.vertices:
                                    colors.append([1,0,1])

                            meshfile.write("%i\n" % (len(mesh.vertices)/9))
                            for i,v in enumerate(mesh.vertices):
                                meshfile.write("\t%f,%f,%f\t%f,%f,%f\t%f,%f,%f\n" % (-v.co.x,v.co.z,v.co.y, -v.normal.x,v.normal.z,v.normal.y, colors[i][0],colors[i][1],colors[i][2]))

                            meshfile.write("%i\n" % len(mesh.tessfaces))
                            for i in mesh.tessfaces:
                                meshfile.write("\t%i\t%i\t%i\n" % (i.vertices[0],i.vertices[1],i.vertices[2]))
                            bpy.data.meshes.remove(mesh)
                            meshfile.close()

                if o.type == 'LAMP':
                    if o.data.type == 'POINT':
                        pointlight = "%f,%f,%f\t%f\t%f\t%f\t%f,%f,%f" % (o.location.x,o.location.z,o.location.y, o.data.energy, o.data.linear_attenuation, o.data.quadratic_attenuation, o.data.color.r,o.data.color.g,o.data.color.b)
                        pointlist.append(pointlight)
                    elif o.data.type == 'SUN':
                        dirlight = "%f,%f,%f\t%f,%f,%f" % (o.rotation_euler.x,o.rotation_euler.z,o.rotation_euler.y, o.data.color.r,o.data.color.g,o.data.color.b)
                        dirlist.append(dirlight)

            meshlist = list(set(meshlist))

            for o in s.objects:
                if o.type == 'EMPTY' and o.dupli_group:
                    # entitycount = "%i\n" % (len(o.dupli_group.objects))
                    # for dgo in o.dupli_group.objects:
                    rot = o.rotation_euler.to_quaternion()
                    entity = "%s\t%f,%f,%f\t%f,%f,%f,%f\t%f,%f,%f" % (meshlist.index(o.dupli_group.objects[0].name), o.location.x,o.location.z,o.location.y, rot.w,rot.x,rot.z,rot.y, o.scale.x,o.scale.z,o.scale.y)
                    entitylist.append(entity)

                    #For puzzle connections
                    #for i in o.data.tessfaces:
                    #    f.write('%i %i %i ' % (i.vertices[0], i.vertices[2], i.vertices[1]))
                    #f.write('\n')

            scenefile.write("%i\n" % len(meshlist))
            for m in meshlist:
                scenefile.write("\t%s\n" % m)

            scenefile.write("%i\n" % (len(entitylist)))
            for e in entitylist:
                scenefile.write("\t%s\n" % e)

            scenefile.write("%i\n" % len(dirlist))
            for d in dirlist:
                scenefile.write("\t%s\n" % d)

            scenefile.write("%i\n" % len(pointlist))
            for p in pointlist:
                scenefile.write("\t%s\n" % p)

            scenefile.write("%f,%f,%f\n" % (s.world.horizon_color.r, s.world.horizon_color.g, s.world.horizon_color.b))

            scenefile.write("%f,%f,%f\n" % (s.world.ambient_color.r, s.world.ambient_color.g, s.world.ambient_color.b))

            scenefile.close()

        print('=====================')
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

    # # test call
    # bpy.ops.export_scene.scene('INVOKE_DEFAULT')
