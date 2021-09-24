#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cvector.h"

typedef struct {
	cvector_vector_type(float) verts;
	cvector_vector_type(float) uvs;
	cvector_vector_type(float) norms;
	
	cvector_vector_type(float) modelVerts;
	cvector_vector_type(float) modelUvs;
	cvector_vector_type(float) modelNormals;
} Loader;

cvector_vector_type(float) loadVertex(char* line, cvector_vector_type(float) list, size_t attribs) {
	char* token = strtok(line, " ");
	size_t tokens = 0;
	
    while(token) {
        if(tokens > 0 && tokens-1 < attribs) cvector_push_back(list, (float) atof(token));
        token = strtok(NULL, " ");
		tokens++;
    }
	
	return list;
}

void loadFace(char* line, Loader* loader) {
	
	size_t vertsCount = 0;
	{
		char* tmp = strchr(line, ' ');
		while(tmp) {
			vertsCount++;
			tmp = strchr(tmp + 1, ' ');
		}
		
		vertsCount--;
	}
	
	char* token = strtok(line, " ");
	size_t tokens = 0;
	size_t vert = 0;
	
    while(token) {
		
        if(tokens > 0) {
			int v = 0, uv = 0, n = 0;
			
			char* tmp = token;
			size_t attrib = 0;
			
			while(tmp) {
				if(attrib == 0) v = atoi(tmp);
				else if(attrib == 1) uv = atoi(tmp);
				else if(attrib == 2) n = atoi(tmp);
				
				attrib++;
				tmp = strchr(tmp, '/');
				if(tmp) tmp++;
			}
			
			if(vert >= 3) {
				//триангуляция многоугольников
				for(int i=0; i<3; i++) cvector_push_back(loader->modelVerts, loader->modelVerts[cvector_size(loader->modelVerts) - 3*vert]);
				for(int i=0; i<3; i++) cvector_push_back(loader->modelVerts, loader->modelVerts[cvector_size(loader->modelVerts) - 6]);
				
				for(int i=0; i<2; i++) cvector_push_back(loader->modelUvs, loader->modelUvs[cvector_size(loader->modelUvs) - 2*vert]);
				for(int i=0; i<2; i++) cvector_push_back(loader->modelUvs, loader->modelUvs[cvector_size(loader->modelUvs) - 4]);
				
				for(int i=0; i<3; i++) cvector_push_back(loader->modelNormals, loader->modelNormals[cvector_size(loader->modelNormals) - 3*vert]);
				for(int i=0; i<3; i++) cvector_push_back(loader->modelNormals, loader->modelNormals[cvector_size(loader->modelNormals) - 6]);
			}
			
			for(int i=0; i<3; i++) cvector_push_back(loader->modelVerts, v <= 0 ? (0) : (loader->verts[(v-1) * 3 + i]));
			for(int i=0; i<2; i++) cvector_push_back(loader->modelUvs, uv <= 0 ? (0) : (loader->uvs[(uv-1) * 2 + i]));
			for(int i=0; i<3; i++) cvector_push_back(loader->modelNormals, n <= 0 ? (0) : (loader->norms[(n-1) * 3 + i]));
			
			vert++;
		}
		
        token = strtok(NULL, " ");
		tokens++;
    }
}

int loadObj(char* file, float** vecs, float** uvs, float** normals) {
	FILE* f = fopen(file, "rt");
	if(!f) {
		printf("Can't open file %s\n", file);
		perror("");
		
		return 1;
	}
	
	char* line = calloc(1024, sizeof(char));
	
	if(!line) {
		printf("Can't allocate line\n");
		fclose(f);
		
		return 1;
	}
	
	Loader loader = (Loader) {0};
	
	while(fgets(line, 1024, f)) {
		size_t size = strlen(line) - 1; //skip new line character
		if(!size) continue;
		
		if(strstr(line, "v ") == line) {
			loader.verts = loadVertex(line, loader.verts, 3);
		} else if(strstr(line, "vt ") == line) {
			loader.uvs = loadVertex(line, loader.uvs, 2);
			loader.uvs[cvector_size(loader.uvs) - 1] = 1 - loader.uvs[cvector_size(loader.uvs) - 1]; //v tex coord fix
		} else if(strstr(line, "vn ") == line) {
			loader.norms = loadVertex(line, loader.norms, 3);
		} else if(strstr(line, "f ") == line) {
			loadFace(line, &loader);
		}
		
	}
	
	free(line);
	fclose(f);
	
	cvector_free(loader.verts);
	cvector_free(loader.uvs);
	cvector_free(loader.norms);
	
	*vecs = loader.modelVerts;
	*uvs = loader.modelUvs;
	*normals = loader.modelNormals;
	
	return 0;
}