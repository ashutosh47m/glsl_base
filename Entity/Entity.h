#ifndef _ENTITY_H
#define _ENTITY_H
class Entity
{
	//pure virtual abstract class
	//virtual void initEntity() = 0;
	//virtual void draw(glsl_data&) = 0;
	
	//enables the entity using glBindVertexArray
	virtual void enable() = 0;
};
#endif
