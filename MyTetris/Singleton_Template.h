/*
	Filename: Singleton_Template.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Template to give SINGLETON functionality to any given class
	Comments: This singleton template doesnt manage the fact that any singleton class could depend 
			  on other singleton implementations, and maybe destruction is not in good order as 
			  depending on implementation of compiler... FOR NOW IT WILL BE LEFT LIKE THAT...
	TODO:
		IMPLEMENT PHOENIX SINGLETON (ADDITIONALLY TO MEYERS)
	Attribution: Based in article by J. Nakamura at http://www.devarticles.com/c/a/Cplusplus/
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _SINGLETON_TEMPLATE
#define _SINGLETON_TEMPLATE

template <class T>
class Singleton
{
public:
	//Main way to access class pointer and instance is by this function
	//(INSTANCE)
	static T* Instance() 
	{
		static T _instance;
		return &_instance;
	}
private:
	//----- TEMPLATE SINGLETON STRUCTURE -------
	//Constructor, copy constructor, destructor, and operator = hidden
	Singleton();
	~Singleton();
	Singleton(Singleton const&);		
	Singleton& operator=(Singleton const&);  // assign op hidden
};

#endif