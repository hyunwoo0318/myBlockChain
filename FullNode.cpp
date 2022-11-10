#include"utilHeader.h"

/*
FullNode

UserNode로부터 TX를 받으면 일단 검증을 한다.
검증에 성공하면 해당 TX를 포함하여 채굴을 시도한다.

각각의 FullNode(개별 proces)는 blockchain을 가지고있다.
*/

