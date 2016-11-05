#include "SphereSolid.h"

int numVerticesSolid = 24;
int numSlicesSolid = numVerticesSolid/4;
vector<glm::vec3> stackPointsSolid;
vector<glm::vec3> slicePointsSolid;
vector<unsigned int> indicesSolid;
int drawTypeSolid = GL_LINES;
const int WIREFRAME = 0;
const int SOLID = 1;

SphereSolid::SphereSolid(GLuint shaderProgram, glm::vec3 color, int mode) : shaderProgram(shaderProgram), color(color), SphereMode(mode)
{

	int linesType = GL_LINES;
	int trianglesType = GL_TRIANGLES;

	float sliceDeg = 180.0f / numSlicesSolid;
	float verticesDeg = 360.0f / numVerticesSolid;

	//Make slices
	for (float y = 0; y < numSlicesSolid; y++)
	{
		for (float x = 0; x < numVerticesSolid; x++)
		{
			glm::vec3 slice = { cos(glm::radians(x * verticesDeg)), sin(glm::radians(x * verticesDeg)), 0.0f };
			slice = glm::vec3(glm::rotate(glm::mat4(1.0f), (y * sliceDeg) / 180.0f * glm::pi<float>(), { 0.0f, 1.0f, 0.0f }) * glm::vec4(slice, 1.0f));

			slicePointsSolid.push_back(slice);
		}
	}

	cout << SphereMode << endl;
	//Wireframe Sphere
	switch (SphereMode)
	{

	case WIREFRAME:
	{
		cout << "WireFrame" << endl;
		drawTypeSolid = linesType;
		//Slices indicesSolid
		for (float y = 0; y < numSlicesSolid; y++)
		{
			for (float x = 0; x < numVerticesSolid; x++)
			{
				indicesSolid.push_back(x + numVerticesSolid * y);
				if (x < numVerticesSolid - 1)
					indicesSolid.push_back((x + 1) + numVerticesSolid * y);
				else
					indicesSolid.push_back(y * numVerticesSolid);
			}
		}

		//Top half stack indicesSolid
		for (float x = 0; x < numVerticesSolid / 4; x++)
		{
			//Draw first half of cirlce
			for (float y = 0; y < numSlicesSolid; y++)
			{
				indicesSolid.push_back(y * numVerticesSolid + x);
				if (y < numSlicesSolid - 1)
					indicesSolid.push_back((y + 1) * numVerticesSolid + x);
				else
					indicesSolid.push_back(0 * numVerticesSolid + numVerticesSolid / 2 - x);
			}

			//Draw second half of circle
			for (float y = 0; y < numSlicesSolid; y++)
			{
				indicesSolid.push_back(y * numVerticesSolid + numVerticesSolid / 2 - x);
				if (y < numSlicesSolid - 1)
					indicesSolid.push_back((y + 1) * numVerticesSolid + numVerticesSolid / 2 - x);
				else
					indicesSolid.push_back(0 * numVerticesSolid + x);
			}
		}

		//Bottom half stack indicesSolid
		for (float x = 1; x < numVerticesSolid / 4; x++)
		{
			//Draw first half of cirlce
			for (float y = 0; y < numSlicesSolid; y++)
			{
				indicesSolid.push_back(y * numVerticesSolid + numVerticesSolid / 2 + x);
				if (y < numSlicesSolid - 1)
					indicesSolid.push_back((y + 1) * numVerticesSolid + numVerticesSolid / 2 + x);
				else
					indicesSolid.push_back(0 * numVerticesSolid + numVerticesSolid - x);
			}

			//Draw second half of circle
			for (float y = 0; y < numSlicesSolid; y++)
			{
				indicesSolid.push_back(y * numVerticesSolid + numVerticesSolid - x);
				if (y < numSlicesSolid - 1)
					indicesSolid.push_back((y + 1) * numVerticesSolid + numVerticesSolid - x);
				else
					indicesSolid.push_back(0 * numVerticesSolid + numVerticesSolid / 2 + x);
			}
		}

		drawingindicesSolid = indicesSolid.size();

		toWorld = glm::mat4(1.0f);

		// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
		glGenVertexArrays(1, &WVAO);
		glGenBuffers(1, &WVBO);
		glGenBuffers(1, &WEBO);

		// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
		// Consider the VAO as a container for all your buffers.
		glBindVertexArray(WVAO);

		// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
		// you want to draw, such as vertices, normals, colors, etc.
		glBindBuffer(GL_ARRAY_BUFFER, WVBO);
		// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
		// the 2nd argument number of indicesSolid. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
		glBufferData(GL_ARRAY_BUFFER, slicePointsSolid.size() * sizeof(slicePointsSolid), &slicePointsSolid[0], GL_STATIC_DRAW);
		// Enable the usage of layout location 0 (check the vertex shader to see what this is)
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
			3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
			GL_FLOAT, // What type these components are
			GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
			3 * sizeof(GLfloat), // Offset between consecutive indicesSolid. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
			(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

						 // We've sent the vertex data over to OpenGL, but there's still something missing.
						 // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, WEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSolid.size() * sizeof(indicesSolid), &indicesSolid[0], GL_STATIC_DRAW);

		// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Unbind the VAO now so we don't accidentally tamper with it.
		// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
		glBindVertexArray(0);
		break;
	}
	
	case SOLID:
	{
		cout << "SOLID" << endl;
		drawTypeSolid = trianglesType;
		//Solid Sphere

		//Slices indicesSolid
		for (float y = 0; y < numSlicesSolid; y++)
		{
			for (float x = 0; x < numVerticesSolid; x++)
			{
				if (y < numSlicesSolid - 1)
				{
					if (x < numVerticesSolid - 1)
					{
						//First triangle
						indicesSolid.push_back(x + numVerticesSolid * y);
						indicesSolid.push_back(x + numVerticesSolid * (y + 1));
						indicesSolid.push_back((x + 1) + numVerticesSolid * y);

						//Second triangle
						indicesSolid.push_back(x + numVerticesSolid * (y + 1));
						indicesSolid.push_back((x + 1) + numVerticesSolid * (y + 1));
						indicesSolid.push_back((x + 1) + numVerticesSolid * y);
					}
					else
					{
						//First triangle
						indicesSolid.push_back(x + numVerticesSolid * y);
						indicesSolid.push_back(x + numVerticesSolid * (y + 1));
						indicesSolid.push_back((0) + numVerticesSolid * y);

						//Second triangle
						indicesSolid.push_back(x + numVerticesSolid * (y + 1));
						indicesSolid.push_back((0) + numVerticesSolid * (y + 1));
						indicesSolid.push_back((0) + numVerticesSolid * y);
					}
				}
				
				else
				{
					if (x < numVerticesSolid / 2)
					{
						//First triangle
						indicesSolid.push_back(x + numVerticesSolid * y);
						indicesSolid.push_back(numVerticesSolid / 2 - x + numVerticesSolid * (0));
						indicesSolid.push_back((x + 1) + numVerticesSolid * y);

						//Second triangle
						indicesSolid.push_back(numVerticesSolid / 2 - x + numVerticesSolid * (0));
						indicesSolid.push_back(numVerticesSolid / 2 - (x + 1) + numVerticesSolid * (0));
						indicesSolid.push_back((x + 1) + numVerticesSolid * y);
					}
					
					else
					{
						if (x < numVerticesSolid - 1)
						{
							//First triangle
							indicesSolid.push_back(x + numVerticesSolid * y);
							indicesSolid.push_back(numVerticesSolid - x + numVerticesSolid * (0));
							indicesSolid.push_back((x + 1) + numVerticesSolid * y);

							//Second triangle
							indicesSolid.push_back(numVerticesSolid - x + numVerticesSolid * (0));
							indicesSolid.push_back(numVerticesSolid - (x + 1) + numVerticesSolid * (0));
							indicesSolid.push_back((x + 1) + numVerticesSolid * y);
						}
						else
						{
							//First triangle
							indicesSolid.push_back(x + numVerticesSolid * y);
							indicesSolid.push_back(numVerticesSolid - x + numVerticesSolid * (0));
							indicesSolid.push_back((0) + numVerticesSolid * y);

							//Second triangle
							indicesSolid.push_back(numVerticesSolid - x + numVerticesSolid * (0));
							indicesSolid.push_back(numVerticesSolid - (0) + numVerticesSolid * (0));
							indicesSolid.push_back((0) + numVerticesSolid * y);
						}
					}	
				}	
			}
		}

		drawingindicesSolid = indicesSolid.size();

		toWorld = glm::mat4(1.0f);

		// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
		glGenVertexArrays(1, &SVAO);
		glGenBuffers(1, &SVBO);
		glGenBuffers(1, &SEBO);

		// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
		// Consider the VAO as a container for all your buffers.
		glBindVertexArray(SVAO);

		// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
		// you want to draw, such as vertices, normals, colors, etc.
		glBindBuffer(GL_ARRAY_BUFFER, SVBO);
		// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
		// the 2nd argument number of indicesSolid. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
		glBufferData(GL_ARRAY_BUFFER, slicePointsSolid.size() * sizeof(slicePointsSolid), &slicePointsSolid[0], GL_STATIC_DRAW);
		// Enable the usage of layout location 0 (check the vertex shader to see what this is)
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
			3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
			GL_FLOAT, // What type these components are
			GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
			3 * sizeof(GLfloat), // Offset between consecutive indicesSolid. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
			(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

						 // We've sent the vertex data over to OpenGL, but there's still something missing.
						 // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSolid.size() * sizeof(indicesSolid), &indicesSolid[0], GL_STATIC_DRAW);

		// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Unbind the VAO now so we don't accidentally tamper with it.
		// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
		glBindVertexArray(0);

		break;
	}

	default :
		break;
	}

	for (int x = 0; x < indicesSolid.size(); x++)
	{
		if(indicesSolid[x] >= slicePointsSolid.size())
		cout <<  x << ": " << indicesSolid[x] << " / " << slicePointsSolid.size() - 1 << endl;
	}


}

SphereSolid::~SphereSolid()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &WVAO);
	glDeleteBuffers(1, &WVBO);
	glDeleteBuffers(1, &WEBO);

	glDeleteVertexArrays(1, &SVAO);
	glDeleteBuffers(1, &SVBO);
	glDeleteBuffers(1, &SEBO);
}

void SphereSolid::render()
{ 

	//cout << color.x << end;
	GLint colorLoc = glGetUniformLocation(shaderProgram, "material.color");
	glUniform3f(colorLoc, color.x, color.y, color.z);

	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	switch (SphereMode)
	{
	case WIREFRAME:

		// Now draw the Sphere. We simply need to bind the VAO associated with it.
		glBindVertexArray(WVAO);
		// Tell OpenGL to draw with triangles, using 36 indicesSolid, the type of the indicesSolid, and the offset to start from
		glDrawElements(drawTypeSolid, drawingindicesSolid, GL_UNSIGNED_INT, 0);
		// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
		glBindVertexArray(0);

		break;
	case SOLID:


		// Now draw the Sphere. We simply need to bind the VAO associated with it.
		glBindVertexArray(SVAO);
		// Tell OpenGL to draw with triangles, using 36 indicesSolid, the type of the indicesSolid, and the offset to start from
		glDrawElements(drawTypeSolid, drawingindicesSolid, GL_UNSIGNED_INT, 0);
		// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
		glBindVertexArray(0);
		break;
	}
}

void SphereSolid::update(bool on, glm::vec3 points[6], glm::vec3 normals[6])
{
	if (SphereMode == WIREFRAME)
	{
		if (on)
			drawingindicesSolid = indicesSolid.size();
		else
			drawingindicesSolid = 0;
	}
}

