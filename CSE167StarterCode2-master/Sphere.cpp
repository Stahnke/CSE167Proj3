#include "Sphere.h"
#include "Window.h"

int numVertices = 24;
int numSlices = numVertices/4;
vector<glm::vec3> stackPoints;
vector<glm::vec3> slicePoints;
vector<unsigned int> indices;
int drawType = GL_LINES;
const int WIREFRAME = 0;
const int SOLID = 1;

Sphere::Sphere(GLuint shaderProgram, glm::vec3 color, int mode) : shaderProgram(shaderProgram), color(color), SphereMode(mode)
{

	int linesType = GL_LINES;
	int trianglesType = GL_TRIANGLES;

	float sliceDeg = 180.0f / numSlices;
	float verticesDeg = 360.0f / numVertices;

	//Make slices
	for (float y = 0; y < numSlices; y++)
	{
		for (float x = 0; x < numVertices; x++)
		{
			glm::vec3 slice = { cos(glm::radians(x * verticesDeg)), sin(glm::radians(x * verticesDeg)), 0.0f };
			slice = glm::vec3(glm::rotate(glm::mat4(1.0f), (y * sliceDeg) / 180.0f * glm::pi<float>(), { 0.0f, 1.0f, 0.0f }) * glm::vec4(slice, 1.0f));

			slicePoints.push_back(slice);
		}
	}

	cout << SphereMode << endl;
	//Wireframe Sphere
	switch (SphereMode)
	{

	case WIREFRAME:
	{
		cout << "WireFrame" << endl;
		drawType = linesType;
		//Slices indices
		for (float y = 0; y < numSlices; y++)
		{
			for (float x = 0; x < numVertices; x++)
			{
				indices.push_back(x + numVertices * y);
				if (x < numVertices - 1)
					indices.push_back((x + 1) + numVertices * y);
				else
					indices.push_back(y * numVertices);
			}
		}

		//Top half stack indices
		for (float x = 0; x < numVertices / 4; x++)
		{
			//Draw first half of cirlce
			for (float y = 0; y < numSlices; y++)
			{
				indices.push_back(y * numVertices + x);
				if (y < numSlices - 1)
					indices.push_back((y + 1) * numVertices + x);
				else
					indices.push_back(0 * numVertices + numVertices / 2 - x);
			}

			//Draw second half of circle
			for (float y = 0; y < numSlices; y++)
			{
				indices.push_back(y * numVertices + numVertices / 2 - x);
				if (y < numSlices - 1)
					indices.push_back((y + 1) * numVertices + numVertices / 2 - x);
				else
					indices.push_back(0 * numVertices + x);
			}
		}

		//Bottom half stack indices
		for (float x = 1; x < numVertices / 4; x++)
		{
			//Draw first half of cirlce
			for (float y = 0; y < numSlices; y++)
			{
				indices.push_back(y * numVertices + numVertices / 2 + x);
				if (y < numSlices - 1)
					indices.push_back((y + 1) * numVertices + numVertices / 2 + x);
				else
					indices.push_back(0 * numVertices + numVertices - x);
			}

			//Draw second half of circle
			for (float y = 0; y < numSlices; y++)
			{
				indices.push_back(y * numVertices + numVertices - x);
				if (y < numSlices - 1)
					indices.push_back((y + 1) * numVertices + numVertices - x);
				else
					indices.push_back(0 * numVertices + numVertices / 2 + x);
			}
		}

		drawingIndices = indices.size();

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
		// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
		glBufferData(GL_ARRAY_BUFFER, slicePoints.size() * sizeof(slicePoints), &slicePoints[0], GL_STATIC_DRAW);
		// Enable the usage of layout location 0 (check the vertex shader to see what this is)
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
			3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
			GL_FLOAT, // What type these components are
			GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
			3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
			(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

						 // We've sent the vertex data over to OpenGL, but there's still something missing.
						 // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, WEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), &indices[0], GL_STATIC_DRAW);

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
		drawType = trianglesType;
		//Solid Sphere

		//Slices indices
		for (float y = 0; y < numSlices; y++)
		{
			for (float x = 0; x < numVertices; x++)
			{
				if (y < numSlices - 1)
				{
					if (x < numVertices - 1)
					{
						//First triangle
						indices.push_back(x + numVertices * y);
						indices.push_back(x + numVertices * (y + 1));
						indices.push_back((x + 1) + numVertices * y);

						//Second triangle
						indices.push_back(x + numVertices * (y + 1));
						indices.push_back((x + 1) + numVertices * (y + 1));
						indices.push_back((x + 1) + numVertices * y);
					}
					else
					{
						//First triangle
						indices.push_back(x + numVertices * y);
						indices.push_back(x + numVertices * (y + 1));
						indices.push_back((0) + numVertices * y);

						//Second triangle
						indices.push_back(x + numVertices * (y + 1));
						indices.push_back((0) + numVertices * (y + 1));
						indices.push_back((0) + numVertices * y);
					}
				}
				
				else
				{
					if (x < numVertices / 2)
					{
						//First triangle
						indices.push_back(x + numVertices * y);
						indices.push_back(numVertices / 2 - x + numVertices * (0));
						indices.push_back((x + 1) + numVertices * y);

						//Second triangle
						indices.push_back(numVertices / 2 - x + numVertices * (0));
						indices.push_back(numVertices / 2 - (x + 1) + numVertices * (0));
						indices.push_back((x + 1) + numVertices * y);
					}
					
					else
					{
						if (x < numVertices - 1)
						{
							//First triangle
							indices.push_back(x + numVertices * y);
							indices.push_back(numVertices - x + numVertices * (0));
							indices.push_back((x + 1) + numVertices * y);

							//Second triangle
							indices.push_back(numVertices - x + numVertices * (0));
							indices.push_back(numVertices - (x + 1) + numVertices * (0));
							indices.push_back((x + 1) + numVertices * y);
						}
						else
						{
							//First triangle
							indices.push_back(x + numVertices * y);
							indices.push_back(numVertices - x + numVertices * (0));
							indices.push_back((0) + numVertices * y);

							//Second triangle
							indices.push_back(numVertices - x + numVertices * (0));
							indices.push_back(numVertices - (0) + numVertices * (0));
							indices.push_back((0) + numVertices * y);
						}
					}	
				}	
			}
		}

		drawingIndices = indices.size();

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
		// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
		glBufferData(GL_ARRAY_BUFFER, slicePoints.size() * sizeof(slicePoints), &slicePoints[0], GL_STATIC_DRAW);
		// Enable the usage of layout location 0 (check the vertex shader to see what this is)
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
			3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
			GL_FLOAT, // What type these components are
			GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
			3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
			(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

						 // We've sent the vertex data over to OpenGL, but there's still something missing.
						 // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), &indices[0], GL_STATIC_DRAW);

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

	for (int x = 0; x < indices.size(); x++)
	{
		if(indices[x] >= slicePoints.size())
		cout <<  x << ": " << indices[x] << " / " << slicePoints.size() - 1 << endl;
	}


}

Sphere::~Sphere()
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

void Sphere::render()
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
		// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
		glDrawElements(drawType, drawingIndices, GL_UNSIGNED_INT, 0);
		// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
		glBindVertexArray(0);

		break;
	case SOLID:


		// Now draw the Sphere. We simply need to bind the VAO associated with it.
		glBindVertexArray(SVAO);
		// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
		glDrawElements(drawType, drawingIndices, GL_UNSIGNED_INT, 0);
		// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
		glBindVertexArray(0);
		break;
	}
}

void Sphere::update(bool on, glm::vec3 points[6], glm::vec3 normals[6])
{
	if (SphereMode == WIREFRAME)
	{
		if (on)
			drawingIndices = indices.size();
		else
			drawingIndices = 0;
	}
}

