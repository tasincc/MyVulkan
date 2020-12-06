import os
import subprocess
glslang = "glslangValidator.exe"
def file_name(file_dir):
    for root, dirs, files in os.walk(file_dir):
    	for file in files: 
    		if file.endswith(".vert") or \
    		file.endswith(".frag"):
    		    glsl_file = os.path.join(root, file)
    		    spv_out = glsl_file + ".spv"
    		    # print('Compiling Success: %s' % (glsl_file))
    		    ret = subprocess.getoutput([glslang,'-V',glsl_file,'-o',spv_out])
    		    print(ret)                

file_name(os.getcwd())
os.system('pause')