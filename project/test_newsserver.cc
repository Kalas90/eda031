#include "newsserver.h"
#include "memorynewsgroupprovider.h"

int main() {
	MemoryNewsgroupProvider mnsg;
	NewsServer ns(1234, mnsg);
	ns.listen();
}