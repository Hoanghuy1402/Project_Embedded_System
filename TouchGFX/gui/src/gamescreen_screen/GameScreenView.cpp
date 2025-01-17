//#include <gui/gamescreen_screen/GameScreenView.hpp>
//#include <C:\TouchGFXProjects\HeNhung\STM32CubeIDE\Application\User\src\gameplay.hpp>
//#include <BitmapDatabase.hpp>
//#include <cmsis_os2.h>
//#include <cmsis_os.h>
//#include <cstring>
//#include <cstdlib>
//#include <cstdlib>
//#include <iostream>
//// Khai báo biến extern và một số biến toàn cục khác
//extern void gameTask(void *argument);
//osThreadId_t gameTaskHandle;
//uint8_t lives = 3;
//bool shouldStopScreen;
//extern osMessageQueueId_t Queue1Handle;
//static uint32_t seed = 12345;
//GameScreenView::GameScreenView()
//{
//    // Khởi tạo đối tượng Game và các thành phần đồ họa trên màn hình game
//	gameInstance = Game();
//	remove(menu_button);
//	remove(score_holder);
//	remove(high_score_holder);
//	// Prepare ship
//	// Chuẩn bị hình ảnh cho tàu và thiết lập vị trí ban đầu
//	shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIP_MAIN_ID));
//	shipImage.setXY(gameInstance.ship.coordinateX, gameInstance.ship.coordinateY);
//	add(shipImage);
//
//    // Chuẩn bị hình ảnh cho đạn của tàu và đạn của kẻ địch, thiết lập vị trí ban đầu và trạng thái ẩn
//	for(int i=0;i<MAX_BULLET;i++) {
//		enemyBulletImage[i].setXY(321, 33);
//		enemyBulletImage[i].setBitmap(touchgfx::Bitmap(BITMAP_ENEMY_BULLET_RED_ID));
//		shipBulletImage[i].setXY(321, 33);
//		shipBulletImage[i].setBitmap(touchgfx::Bitmap(BITMAP_BULLET_DOUBLE_ID));
//	}
//
//	// Chuẩn bị hình ảnh cho các kẻ địch, sử dụng chuyển đổi màu cho từng loại kẻ địch và thiết lập thời gian cập nhật hình ảnh
//	for(int i=0;i<MAX_ENEMY;i++) {
//		seed = (1103515245 * seed + 12345) % 0x7FFFFFFF;
//		switch(seed%3) {
//		case 0:
//			enemyImage[i].setBitmaps(BITMAP_ENEMY_GREEN_01_ID, BITMAP_ENEMY_GREEN_02_ID);
//			break;
//		case 1:
//			enemyImage[i].setBitmaps(BITMAP_ENEMY_BLUE_01_ID, BITMAP_ENEMY_BLUE_02_ID);
//			break;
//		case 2:
//			enemyImage[i].setBitmaps(BITMAP_ENEMY_PURPLE_01_ID, BITMAP_ENEMY_PURPLE_02_ID);
//			break;
//		default:
//			break;
//		}
//		// thiết lập khoảng thời gian giữa 2 lần cập nhật hình ảnh
//		enemyImage[i].setUpdateTicksInterval(20);
//	}
//	bossImage.setXY(321, 33);
//	bossImage.setBitmap(touchgfx::Bitmap(BITMAP_MYSTERY_ID));
//
//}
//
//void GameScreenView::setupScreen()
//{
//	GameScreenViewBase::setupScreen();
//
//	// Kết thúc task game trước khi bắt đầu màn hình game mới (đảm bảo không có task game nào đang chạy)
//	osThreadTerminate(gameTaskHandle);
//	const osThreadAttr_t gameTask_attributes = {
//	  .name = "gameTask", // Tên của task là "gameTask"
//	  .stack_size = 8192 * 2,  // Kích thước của stack được cấp phát cho task là 8192 * 2 bytes
//	  .priority = (osPriority_t) osPriorityNormal, // Ưu tiên của task được đặt là osPriorityNormal (ưu tiên bình thường)
//	};
//
//	// Tạo một task mới có các thuộc tính đã được khai báo trước đó
//	gameTaskHandle = osThreadNew(gameTask, NULL, &gameTask_attributes);
//	shouldEndGame = false;
//	shouldStopTask = false;
//	shouldStopScreen = false;
//}
//
//void GameScreenView::tearDownScreen()
//{
//    GameScreenViewBase::tearDownScreen();
//}
//
//uint8_t getQueue()
//{
//	if (osMessageQueueGetCount(Queue1Handle) > 0) {
//		uint8_t res;
//		osMessageQueueGet(Queue1Handle, &res, 0, osWaitForever);
//		return res;
//	}
//	else {
//		return 0;
//	}
//}
//
//
//// Render game objects
//void GameScreenView::handleTickEvent() {
//	GameScreenViewBase::handleTickEvent();
//
//	if(shouldEndGame && shouldStopTask && !shouldStopScreen) {
//		add(menu_button);
//		Unicode::snprintf(score_holderBuffer, SCORE_HOLDER_SIZE, "%d", gameInstance.score);
//		add(score_holder);
//		Unicode::snprintf(high_score_holderBuffer, HIGH_SCORE_HOLDER_SIZE, "%d", gameInstance.high_score);
//		add(high_score_holder);
//		menu_button.invalidate();
//		score_holder.invalidate();
//		high_score_holder.invalidate();
//
//		shouldStopScreen = true;
//    osThreadTerminate(gameTaskHandle);
//	}
//
//	uint8_t move = getQueue();
//	if (move == 'R') {
//		gameInstance.ship.updateVelocityX(gameInstance.ship.VELOCITY);
//		shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIP_MAIN_ID));
//		osMessageQueueReset(Queue1Handle);
//	}
//	if (move == 'L') {
//		gameInstance.ship.updateVelocityX(-gameInstance.ship.VELOCITY);
//		shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIP_MAIN_ID));
//		osMessageQueueReset(Queue1Handle);
//	}
//	if (move == 'U') {
//		gameInstance.ship.updateVelocityY(gameInstance.ship.VELOCITY);
//		osMessageQueueReset(Queue1Handle);
//	}
//	if (move == 'D') {
//		gameInstance.ship.updateVelocityY(-gameInstance.ship.VELOCITY);
//		osMessageQueueReset(Queue1Handle);
//	}
//
//	// update player position and sprite
//	shipImage.moveTo(gameInstance.ship.coordinateX, gameInstance.ship.coordinateY);
//
//	// render ship bullet
//	for(int i=0; i<MAX_BULLET;i++) {
//		switch(shipBullet[i].displayStatus){
//		case IS_HIDDEN:
//			break;
//		case IS_SHOWN:
//			shipBulletImage[i].moveTo(shipBullet[i].coordinateX, shipBullet[i].coordinateY);
//			break;
//		case SHOULD_SHOW:
//			shipBulletImage[i].moveTo(shipBullet[i].coordinateX, shipBullet[i].coordinateY);
//			add(shipBulletImage[i]);
//			shipBullet[i].updateDisplayStatus(IS_SHOWN);
//			break;
//		case SHOULD_HIDE:
//			remove(shipBulletImage[i]);
//			shipBullet[i].updateDisplayStatus(IS_HIDDEN);
//			break;
//		default:
//			break;
//		}
//	}
//
//	// render enemy bullet
//	for(int i=0; i<MAX_BULLET;i++) {
//		switch(enemyBullet[i].displayStatus){
//		case IS_HIDDEN:
//			break;
//		case IS_SHOWN:
//			enemyBulletImage[i].moveTo(enemyBullet[i].coordinateX, enemyBullet[i].coordinateY);
//			break;
//		case SHOULD_SHOW:
//			enemyBulletImage[i].moveTo(enemyBullet[i].coordinateX, enemyBullet[i].coordinateY);
//			add(enemyBulletImage[i]);
//			enemyBullet[i].updateDisplayStatus(IS_SHOWN);
//			break;
//		case SHOULD_HIDE:
//			remove(enemyBulletImage[i]);
//			enemyBullet[i].updateDisplayStatus(IS_HIDDEN);
//			break;
//		default:
//			break;
//		}
//	}
//
//	// render enemy
//	for(int i=0; i<MAX_ENEMY;i++) {
//		switch(enemy[i].displayStatus){
//		case IS_SHOWN:
//			enemyImage[i].moveTo(enemy[i].coordinateX, enemy[i].coordinateY);
//			break;
//		case SHOULD_SHOW:
//			enemyImage[i].moveTo(enemy[i].coordinateX, enemy[i].coordinateY);
//			enemyImage[i].startAnimation(false, true, true);
//			add(enemyImage[i]);
//			enemy[i].updateDisplayStatus(IS_SHOWN);
//			break;
//		case SHOULD_HIDE:
//			enemyImage[i].stopAnimation();
//			remove(enemyImage[i]);
//			enemy[i].updateDisplayStatus(IS_HIDDEN);
//			break;
////		case SHOULD_EXPLODE:
////			enemyImage[i].setBitmapEnd(BITMAP_EXPLOSIONGREEN_ID);
////			enemyImage[i].stopAnimation();
////			remove(enemyImage[i]);
////			enemy[i].updateDisplayStatus(IS_HIDDEN);
////			break;
//		case IS_HIDDEN:
//		default:
//			break;
//		}
//	}
//
//	// render boss
//	switch(bossInstance.displayStatus) {
//        case IS_SHOWN:
//            bossImage.moveTo(bossInstance.coordinateX, bossInstance.coordinateY);
//            break;
//        case SHOULD_SHOW:
//            bossImage.moveTo(bossInstance.coordinateX, bossInstance.coordinateY);
//            add(bossImage);
//            bossInstance.updateDisplayStatus(IS_SHOWN);
//            break;
//        case SHOULD_HIDE:
//            remove(bossImage);
//            bossInstance.updateDisplayStatus(IS_HIDDEN);
//            break;
//        case SHOULD_EXPLODE:
//            bossImage.setBitmap(BITMAP_EXPLOSIONGREEN_ID);
//            remove(bossImage);
//            bossInstance.updateDisplayStatus(IS_HIDDEN);
//            break;
//        case IS_HIDDEN:
//        default:
//            break;
//    }
//
//
//	// check if player is out of health and should end game
//	if(gameInstance.ship.lives != lives) {
//		lives = gameInstance.ship.lives;
//		if(lives >= 1) {
//			Unicode::snprintf(lives_boardBuffer, LIVES_BOARD_SIZE, "%d", lives);
//		};
//		if(lives >= 2) {
//			Unicode::snprintf(lives_boardBuffer, LIVES_BOARD_SIZE, "%d", lives);
//		};
//		if(lives >= 3) {
//			Unicode::snprintf(lives_boardBuffer, LIVES_BOARD_SIZE, "%d", lives);
//		};
//		// If player is out of health
//		if(lives < 1) {
//			Unicode::snprintf(lives_boardBuffer, LIVES_BOARD_SIZE, "%d", 0);
//			shouldStopTask = true;
//			invalidate();
//		}
//	}
//	Unicode::snprintf(score_boardBuffer, SCORE_BOARD_SIZE, "%d", gameInstance.score);
//	invalidate();
//}
//

#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <C:\TouchGFXProjects\HeNhung\STM32CubeIDE\Application\User\src\gameplay.hpp>
#include <BitmapDatabase.hpp>
#include <cmsis_os2.h>
#include <cmsis_os.h>
#include <cstring>
#include <cstdlib>
#include <cstdlib>
#include <iostream>


// Khai báo biến extern và một số biến toàn cục khác
extern void Outro();
extern void gameTask(void *argument);
osThreadId_t gameTaskHandle;
uint8_t lives = 3;
bool shouldStopScreen;
extern osMessageQueueId_t Queue1Handle;
extern osMessageQueueId_t Queue2Handle;
extern osMessageQueueId_t Queue3Handle;
extern osMessageQueueId_t Queue4Handle;
static uint32_t seed = 12345;
GameScreenView::GameScreenView()
{
    // Khởi tạo đối tượng Game và các thành phần đồ họa trên màn hình game
	gameInstance = Game();
	remove(menu_button);
	remove(score_holder);
	remove(high_score_holder);
	// Prepare ship
	// Chuẩn bị hình ảnh cho tàu và thiết lập vị trí ban đầu
	shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIP_MAIN_ID));
	shipImage.setXY(gameInstance.ship.coordinateX, gameInstance.ship.coordinateY);
	add(shipImage);

    // Chuẩn bị hình ảnh cho đạn của tàu và đạn của kẻ địch, thiết lập vị trí ban đầu và trạng thái ẩn
	for(int i=0;i<MAX_BULLET;i++) {
		enemyBulletImage[i].setXY(321, 33);
		enemyBulletImage[i].setBitmap(touchgfx::Bitmap(BITMAP_ENEMY_BULLET_RED_ID));
		shipBulletImage[i].setXY(321, 33);
		shipBulletImage[i].setBitmap(touchgfx::Bitmap(BITMAP_BULLET_DOUBLE_ID));
	}

	// Chuẩn bị hình ảnh cho các kẻ địch, sử dụng chuyển đổi màu cho từng loại kẻ địch và thiết lập thời gian cập nhật hình ảnh
	for(int i=0;i<MAX_ENEMY;i++) {
		seed = (1103515245 * seed + 12345) % 0x7FFFFFFF;
		switch(seed%3) {
		case 0:
			enemyImage[i].setBitmaps(BITMAP_ENEMY_GREEN_01_ID, BITMAP_ENEMY_GREEN_02_ID);
			break;
		case 1:
			enemyImage[i].setBitmaps(BITMAP_ENEMY_BLUE_01_ID, BITMAP_ENEMY_BLUE_02_ID);
			break;
		case 2:
			enemyImage[i].setBitmaps(BITMAP_ENEMY_PURPLE_01_ID, BITMAP_ENEMY_PURPLE_02_ID);
			break;
		default:
			break;
		}
		// thiết lập khoảng thời gian giữa 2 lần cập nhật hình ảnh
		enemyImage[i].setUpdateTicksInterval(20);
	}
	bossImage.setXY(321, 33);
	bossImage.setBitmap(touchgfx::Bitmap(BITMAP_MYSTERY_ID));

}

void GameScreenView::setupScreen()
{
	GameScreenViewBase::setupScreen();

	// Kết thúc task game trước khi bắt đầu màn hình game mới (đảm bảo không có task game nào đang chạy)
	osThreadTerminate(gameTaskHandle);
	const osThreadAttr_t gameTask_attributes = {
	  .name = "gameTask", // Tên của task là "gameTask"
	  .stack_size = 8192 * 2,  // Kích thước của stack được cấp phát cho task là 8192 * 2 bytes
	  .priority = (osPriority_t) osPriorityNormal, // Ưu tiên của task được đặt là osPriorityNormal (ưu tiên bình thường)
	};

	// Tạo một task mới có các thuộc tính đã được khai báo trước đó
	gameTaskHandle = osThreadNew(gameTask, NULL, &gameTask_attributes);
	shouldEndGame = false;
	shouldStopTask = false;
	shouldStopScreen = false;
}

void GameScreenView::tearDownScreen()
{
    GameScreenViewBase::tearDownScreen();
}

uint8_t getQueue()
{
	if (osMessageQueueGetCount(Queue1Handle) > 0) {
		uint8_t res;
		osMessageQueueGet(Queue1Handle, &res, 0, osWaitForever);
		return res;
	}
	else {
		return 0;
	}
}






// Render game objects
void GameScreenView::handleTickEvent() {
	GameScreenViewBase::handleTickEvent();

	if(shouldEndGame && shouldStopTask && !shouldStopScreen) {
		Outro();
		add(menu_button);
		Unicode::snprintf(score_holderBuffer, SCORE_HOLDER_SIZE, "%d", gameInstance.score);
		add(score_holder);
		Unicode::snprintf(high_score_holderBuffer, HIGH_SCORE_HOLDER_SIZE, "%d", gameInstance.high_score);
		add(high_score_holder);
		menu_button.invalidate();
		score_holder.invalidate();
		high_score_holder.invalidate();

		shouldStopScreen = true;
    osThreadTerminate(gameTaskHandle);
	}

	uint8_t move = getQueue();
	if (move == 'R') {
		gameInstance.ship.updateVelocityX(gameInstance.ship.VELOCITY);
		shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIP_MAIN_ID));
		osMessageQueueReset(Queue1Handle);
	}
	if (move == 'L') {
		gameInstance.ship.updateVelocityX(-gameInstance.ship.VELOCITY);
		shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIP_MAIN_ID));
		osMessageQueueReset(Queue1Handle);
	}
	if (move == 'U') {
		gameInstance.ship.updateVelocityY(gameInstance.ship.VELOCITY);
		osMessageQueueReset(Queue1Handle);
	}
	if (move == 'D') {
		gameInstance.ship.updateVelocityY(-gameInstance.ship.VELOCITY);
		osMessageQueueReset(Queue1Handle);
	}

	// update player position and sprite
	shipImage.moveTo(gameInstance.ship.coordinateX, gameInstance.ship.coordinateY);

	// render ship bullet
	for(int i=0; i<MAX_BULLET;i++) {
		switch(shipBullet[i].displayStatus){
		case IS_HIDDEN:
			break;
		case IS_SHOWN:
			shipBulletImage[i].moveTo(shipBullet[i].coordinateX, shipBullet[i].coordinateY);
			break;
		case SHOULD_SHOW:
			shipBulletImage[i].moveTo(shipBullet[i].coordinateX, shipBullet[i].coordinateY);
			add(shipBulletImage[i]);
			shipBullet[i].updateDisplayStatus(IS_SHOWN);
			break;
		case SHOULD_HIDE:
			remove(shipBulletImage[i]);
			shipBullet[i].updateDisplayStatus(IS_HIDDEN);
			break;
		default:
			break;
		}
	}

	// render enemy bullet
	for(int i=0; i<MAX_BULLET;i++) {
		switch(enemyBullet[i].displayStatus){
		case IS_HIDDEN:
			break;
		case IS_SHOWN:
			enemyBulletImage[i].moveTo(enemyBullet[i].coordinateX, enemyBullet[i].coordinateY);
			break;
		case SHOULD_SHOW:
			enemyBulletImage[i].moveTo(enemyBullet[i].coordinateX, enemyBullet[i].coordinateY);
			add(enemyBulletImage[i]);
			enemyBullet[i].updateDisplayStatus(IS_SHOWN);
			break;
		case SHOULD_HIDE:
			remove(enemyBulletImage[i]);
			enemyBullet[i].updateDisplayStatus(IS_HIDDEN);
			break;
		default:
			break;
		}
	}

	// render enemy
	for(int i=0; i<MAX_ENEMY;i++) {
		switch(enemy[i].displayStatus){
		case IS_SHOWN:
			enemyImage[i].moveTo(enemy[i].coordinateX, enemy[i].coordinateY);
			break;
		case SHOULD_SHOW:
			enemyImage[i].moveTo(enemy[i].coordinateX, enemy[i].coordinateY);
			enemyImage[i].startAnimation(false, true, true);
			add(enemyImage[i]);
			enemy[i].updateDisplayStatus(IS_SHOWN);
			break;
		case SHOULD_HIDE:
			enemyImage[i].stopAnimation();
			remove(enemyImage[i]);
			enemy[i].updateDisplayStatus(IS_HIDDEN);
			break;
//		case SHOULD_EXPLODE:
//			enemyImage[i].setBitmapEnd(BITMAP_EXPLOSIONGREEN_ID);
//			enemyImage[i].stopAnimation();
//			remove(enemyImage[i]);
//			enemy[i].updateDisplayStatus(IS_HIDDEN);
//			break;
		case IS_HIDDEN:
		default:
			break;
		}
	}

	// render boss
	switch(bossInstance.displayStatus) {
        case IS_SHOWN:
            bossImage.moveTo(bossInstance.coordinateX, bossInstance.coordinateY);
            break;
        case SHOULD_SHOW:
            bossImage.moveTo(bossInstance.coordinateX, bossInstance.coordinateY);
            add(bossImage);
            bossInstance.updateDisplayStatus(IS_SHOWN);
            break;
        case SHOULD_HIDE:
            remove(bossImage);
            bossInstance.updateDisplayStatus(IS_HIDDEN);
            break;
//        case SHOULD_EXPLODE:
//            bossImage.setBitmap(BITMAP_EXPLOSIONGREEN_ID);
//            remove(bossImage);
//            bossInstance.updateDisplayStatus(IS_HIDDEN);
//            break;
        case IS_HIDDEN:
        default:
            break;
    }


	// check if player is out of health and should end game
	if(gameInstance.ship.lives != lives) {
		lives = gameInstance.ship.lives;
		if(lives >= 1) {
			Unicode::snprintf(lives_boardBuffer, LIVES_BOARD_SIZE, "%d", lives);
		};
		if(lives >= 2) {
			Unicode::snprintf(lives_boardBuffer, LIVES_BOARD_SIZE, "%d", lives);
		};
		if(lives >= 3) {
			Unicode::snprintf(lives_boardBuffer, LIVES_BOARD_SIZE, "%d", lives);
		};
		// If player is out of health
		if(lives < 1) {
			Unicode::snprintf(lives_boardBuffer, LIVES_BOARD_SIZE, "%d", 0);
			shouldStopTask = true;
			invalidate();
		}
	}
	Unicode::snprintf(score_boardBuffer, SCORE_BOARD_SIZE, "%d", gameInstance.score);
	invalidate();
}
