#include "GameEngine.h"

// f12 moves into defenition   
// f9 breakpoint 
// 1280, 720
// 1536, 896

int main(int argc, char* argv[]) {

	GameEngine* a = new GameEngine(1536, 896,"Le Narrateur", 60);
	while (a->alive()) {
		a->update();
	}
	

	return(0);

}




//14
//https://10fastfingers.com/share-badge/1_CI nice 