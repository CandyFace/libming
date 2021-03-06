#include <mingpp.h>
#include <cstdlib>


int main()
{
	SWFMovieClip *mc;
	SWFMovie *m;
	SWFAction *a;
	
	try {
		m = new SWFMovie();
		a = new SWFAction("trace('');");
		mc = new SWFMovieClip();
	
		mc->addInitAction(a);
		m->addExport(mc, "test");	
	
		m->save("test02.swf");
	}
	catch(SWFException &e)
	{
		std::cerr << "SWFException: " << e.what() << std::endl << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}
