struct Mesh {
	B32 buffered;

	U32 vao;
	U32 vbo;
	U32 ebo;
};


internal Mesh
gpu_buffer_mesh(F32 *g, U32 *c, U32 v, U32 f)
{
	U32 vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, v * 5 * sizeof(F32), g, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, f * 3 * sizeof(U32), c,
		     GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(F32),
			      (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(F32),
			      (void*)(3 * sizeof(F32)));
	glEnableVertexAttribArray(1);


	return { true, vao, vbo, ebo };
}

internal void
mesh_die(Mesh *mesh)
{
	if (mesh->buffered) {
		glDeleteVertexArrays(1, &mesh->vao);
		glDeleteBuffers(1, &mesh->vbo);
		glDeleteBuffers(1, &mesh->ebo);
	}
}

