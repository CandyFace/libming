#include <mingpp.h>
#include <cstdlib>

int main()
{
	try {
		SWFMovie *m = new SWFMovie(8);
		SWFGradient *g = new SWFGradient();

		g->addEntry(0, 0, 0, 0, 255);	
		g->addEntry(1, 255, 255, 255, 255);	

		SWFFillStyle *fs =  SWFFillStyle::GradientFillStyle(g, SWFFILL_LINEAR_GRADIENT);
		SWFShape *shape = new SWFShape();
		shape->setRightFillStyle(fs);
		SWFFill *fill = new SWFFill(fs);
		fill->moveTo(50, 50);	
		shape->setLine(1, 0,0,0,255);
		shape->drawLine(100, 0);
		shape->drawLine(0, 100);
		shape->drawLine(-100, 0);
		shape->drawLine(0, -100);

		m->add(shape);
		m->save("test09.swf");
	}
	catch(SWFException &e)
	{
		std::cerr << "SWFException: " << e.what() << std::endl << std::endl;
		return EXIT_FAILURE;
	}
	return 0;

}
