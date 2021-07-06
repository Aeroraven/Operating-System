#include"EDMain.h"

int main(int argc,char** argv) {
	//Tackling commandlines
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			std::string argv1;
			argv1 = argv[i];
			if (argv1 == "-low-quality") {
				EDGlobal::edglAnimationQuality = EDGLM_LOWQUALITY;
			}
			if (argv1 == "-grayscale") {
				EDGlobal::edglColorMode = EDGLM_GRAYSCALE;
			}
			if (argv1 == "-dark-mode") {
				EDGlobal::edglColorMultiplier = 0.5;
			}
		}
	}

	//Main procedure
	EDMain elevator_dispatch;
	elevator_dispatch.edmStart(&argc,argv);
	return 0;
}