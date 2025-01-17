/*
 * gameplay.cpp
 */

#include "gameplay.hpp"
#include "main.h"
#include <cmsis_os2.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "stack_macros.h"
#include <cstdlib>
#include <ctime>   // Để sử dụng time()

// Ship class implementation
Ship::Ship() {
	this->coordinateX = 104;
	this->coordinateY = 260;
	this->velocityX = 0;
	this->velocityY = 0;
	this->width = 15;
	this->height = 15;
	this->fireRate = FIRE_RATE;
	this->moveRate = MOVE_RATE;
}

Ship::~Ship() {
}

bool Ship::update(uint8_t dt) {
	this->moveRate -= dt;
	if(this->moveRate > 0) return false;
	this->coordinateX += this->velocityX;
	this->coordinateY += this->velocityY;
	this->moveRate = MOVE_RATE;

	if(this->coordinateX < 0) this->coordinateX = 0;
	else if(this->coordinateX > 208) this->coordinateX = 208;
	if(this->coordinateY < 0) this->coordinateY = 0;
	else if(this->coordinateY > 288) this->coordinateY = 288;

	return true;
}

bool Ship::updateBullet(uint8_t dt) {
	this->fireRate -= dt;
	if(this->fireRate > 0) return false;
	this->fireRate = FIRE_RATE;
	return true;
}

void Ship::updateShipHp(short change) {
	this->lives += change;
}

void Ship::updateVelocityX(short X) {
	if (this->coordinateX + X >= 0 && this->coordinateX + X <= 240 - this->width) {
		this->coordinateX += X;
	}
}

void Ship::updateVelocityY(short Y) {
	if (this->coordinateY + Y >= 0 && this->coordinateY + Y <= 320 - this->height){
		this->coordinateY += Y;
	}
}

// Boss class implementation
Boss::Boss() {
	this->coordinateX = 104;
	this->coordinateY = 40;
	this->velocityX = 1;
	this->velocityY = 1;
	this->width = 40;
	this->height = 40;
	this->fireRate = FIRE_RATE;
	this->moveRate = MOVE_RATE;
}

Boss::~Boss() {
}

bool Boss::update(uint8_t dt) {
	this->moveRate -= dt;
	if(this->moveRate > 0) return false;
	this->coordinateX += this->velocityX;
	this->coordinateY += this->velocityY;
	this->moveRate = MOVE_RATE;

	if(this->coordinateX < 0) this->coordinateX = 0;
	else if(this->coordinateX > 208) this->coordinateX = 208;
	if(this->coordinateY < 0) this->coordinateY = 0;
	else if(this->coordinateY > 288) this->coordinateY = 288;

	return true;
}

bool Boss::updateBullet(uint8_t dt) {
	this->fireRate -= dt;
	if(this->fireRate > 0) return false;
	this->fireRate = FIRE_RATE;
	return true;
}

void Boss::updateBossHp(short change) {
	this->lives += change;
}

void Boss::updateVelocityX(short X) {
	this->velocityX = X;
}

void Boss::updateVelocityY(short Y) {
	this->velocityY = Y;
}

// Game class implementation
Game::Game() {
}

Game::~Game() {
}

void Game::updateScore(short score) {
	this->score += score;
}

void Game::updateHighScore(short score) {
	if(score > high_score){
		high_score = score;
	}
}

void Game::update() {
}

// Entity class implementation
Entity::Entity() {
	this->displayStatus = IS_HIDDEN;
	this->width = 32;
	this->height = 32;
}

Entity::~Entity() {
}

bool Entity::update() {
	return false;
}

void Entity::updateDisplayStatus(uint8_t status) {
	this->displayStatus = status;
}

void Entity::updateCoordinate(uint16_t x, uint16_t y) {
	this->coordinateX = x;
	this->coordinateY = y;
}

void Entity::updateVelocity(uint16_t x, uint16_t y) {
	this->velocityX = x;
	this->velocityY = y;
}

bool Entity::isCollide(Entity &a, Entity &b) {
	if((a.coordinateX + a.width > b.coordinateX) && (b.coordinateX + b.width > a.coordinateX) &&
	   (a.coordinateY + a.height > b.coordinateY) && (b.coordinateY + b.height > a.coordinateY))
		return true;
	return false;
}

// Enemy class implementation
Enemy::Enemy() {
	this->width = 32;
	this->height = 32;
	this->displayStatus = IS_HIDDEN;
	this->velocityX = 0;
	this->velocityY = 0;
	this->moveRate = MOVE_RATE;
	this->fireRate = FIRE_RATE;
}

Enemy::~Enemy() {
}

bool Enemy::update(uint8_t dt) {
	if(this->coordinateX < -this->width || this->coordinateX > 240 ||
	   this->coordinateY < -this->height || this->coordinateX > 320) {
		this->displayStatus = SHOULD_HIDE;
		this->fireRate = FIRE_RATE;
		this->moveRate = MOVE_RATE;
		return false;
	}

	this->moveRate -= dt;
	if(this->moveRate > 0) return false;

	this->coordinateX += this->velocityX;
	this->coordinateY += this->velocityY;
	this->moveRate = MOVE_RATE;

	return true;
}

bool Enemy::updateBullet(uint8_t dt) {
	this->fireRate -= dt;
	if(this->fireRate > 0) return false;
	this->fireRate = FIRE_RATE;
	return true;
}

// Bullet class implementation
Bullet::Bullet() {
	this->width = 24;
	this->height = 24;
	this->displayStatus = IS_HIDDEN;
	this->velocityX = 0;
	this->velocityY = 0;
	this->moveRate = MOVE_RATE;
}

Bullet::~Bullet() {
}

bool Bullet::update(uint8_t dt) {
	if(this->coordinateX < -this->width || this->coordinateX > 240 ||
	   this->coordinateY < -this->height || this->coordinateY > 320) {
		this->displayStatus = SHOULD_HIDE;
		this->moveRate = MOVE_RATE;
		return false;
	}

	this->moveRate -= dt;
	if(this->moveRate > 0) return false;

	this->coordinateX += this->velocityX;
	this->coordinateY += this->velocityY;
	this->moveRate = MOVE_RATE;

	return true;
}

// App implementation
Game gameInstance;
Bullet shipBullet[MAX_BULLET];
Bullet enemyBullet[MAX_BULLET];
Enemy enemy[MAX_ENEMY];
Boss bossInstance;
#define MAX_SPAWN_RATE 10000
int spawnRate = MAX_SPAWN_RATE;
int spawnSeed = 0;
uint16_t Game::high_score = 0;

extern TIM_HandleTypeDef htim1;

uint16_t sx, sy;
bool shouldEndGame;
bool shouldStopTask;

void updateEnemy(uint8_t dt);
void updateShipBullet(uint8_t dt);
void updateEnemyBullet(uint8_t dt);

uint8_t dt = 0;
static uint16_t spawnTimer = 0;          // Bộ đếm thời gian để sinh kẻ địch
static int currentWave = 1;              // Wave hiện tại
static int enemiesSpawnedInWave = 0;     // Số lượng kẻ địch đã sinh trong wave hiện tại
static uint16_t waveOffsetX = 0;         // Dịch chuyển ngang theo wave
static uint16_t waveOffsetY = 0;         // Dịch chuyển dọc theo wave
// Thêm hàm reset


#define BOSS_SPAWN_WAVE 5 // Wave mà boss xuất hiện      // Máu của boss
void Boss::resetBoss() {
    bossSpawnTimer = 0; // Reset bộ đếm thời gian
    lives = 20;          // Reset máu của boss
}

// Cập nhật hàm reset để reset thông tin của boss
void resetGame() {
    waveOffsetX = 0;
    waveOffsetY = 0;
    currentWave = 1;
    enemiesSpawnedInWave = 0;
    spawnTimer = 0;
    bossInstance.updateCoordinate(-50, -50); // Đặt boss ngoài màn hình
    bossInstance.updateDisplayStatus(IS_HIDDEN);
    bossInstance.resetBoss();
}

// htim là PA9 htim đó, channel là kênh phát âm thanh (ở đây được cấu hình là TIM_CHANNEL_2), fr là tần số âm thanh muốn phát ra, duration là khoảng th�?i gian
void tone(TIM_HandleTypeDef *htim, uint32_t channel, uint16_t frequency, uint16_t duration) {
    // Tính toán giá trị Period và Pulse dựa trên tần số
    uint32_t period = 1000000/ frequency;

    // Giảm độ rộng xung để giảm âm lượng
    uint32_t pulse = period / 2;  // 50% xung cao

    // Cấu hình chu kỳ (ARR) và độ rộng xung (CCR)
    __HAL_TIM_SET_AUTORELOAD(htim, period-1);
    __HAL_TIM_SET_COMPARE(htim, channel, pulse);

    // Bắt đầu PWM
    HAL_TIM_PWM_Start(htim, channel);

    //
    HAL_Delay(duration);

    //
    HAL_TIM_PWM_Stop(htim, channel);
}

void updateBoss(uint8_t dt) {
    bool allEnemiesHidden = true;

    // Kiểm tra nếu tất cả kẻ địch đều ẩn
    for (int i = 0; i < MAX_ENEMY; i++) {
        if (enemy[i].displayStatus == IS_SHOWN) {
            allEnemiesHidden = false;
            bossInstance.bossSpawnTimer = 0; // Reset timer nếu có kẻ địch hiện
            break;
        }
    }

    // Tăng bộ đếm thời gian nếu tất cả kẻ địch đều ẩn
    if (allEnemiesHidden) {
        bossInstance.bossSpawnTimer += dt;
    }

    // Hiển thị boss nếu tất cả kẻ địch ẩn trong 5 giây
    if (bossInstance.bossSpawnTimer >= 50000 && bossInstance.displayStatus == IS_HIDDEN) {
        bossInstance.updateCoordinate(104, 40); // Tọa độ xuất hiện
        bossInstance.updateVelocity(1, 0);      // Bắt đầu di chuyển sang phải
        bossInstance.updateDisplayStatus(SHOULD_SHOW);
        bossInstance.lives = 20;
        bossInstance.bossSpawnTimer = 0; // Reset timer sau khi boss xuất hiện
    }

    // Nếu boss chưa được hiển thị, thoát ra
    if (bossInstance.displayStatus != IS_SHOWN) return;

    // Kiểm tra biên để thay đổi hướng di chuyển
    const uint16_t leftBoundary = bossInstance.width;  // Giới hạn bên trái
    const uint16_t rightBoundary = 240 - bossInstance.width; // Giới hạn bên phải

    // Đổi hướng nếu boss chạm biên
    if (bossInstance.coordinateX <= leftBoundary && bossInstance.velocityX < 0) {
        bossInstance.updateVelocity(1, 0); // Đổi hướng sang phải
    } else if (bossInstance.coordinateX >= rightBoundary && bossInstance.velocityX > 0) {
        bossInstance.updateVelocity(-1, 0); // Đổi hướng sang trái
    }

    // Cập nhật vị trí và trạng thái của boss
    bossInstance.update(dt);

    // Kiểm tra va chạm với đạn của người chơi
    for (int i = 0; i < MAX_BULLET; i++) {
        if (shipBullet[i].displayStatus != IS_SHOWN) continue;
        if (Entity::isCollide(bossInstance, shipBullet[i])) {
            bossInstance.updateBossHp(-1);
            shipBullet[i].updateDisplayStatus(SHOULD_HIDE);

            if (bossInstance.lives <= 0) {
            	tone(&htim1, TIM_CHANNEL_2, 400, 30);
                bossInstance.updateDisplayStatus(SHOULD_HIDE);
                bossInstance.bossSpawnTimer = 0;
                gameInstance.updateScore(50); // Thưởng điểm khi tiêu diệt boss
            }
            break;
        }
    }

    if(bossInstance.displayStatus == IS_SHOWN){
    	if (Entity::isCollide(bossInstance, gameInstance.ship)) {
    		tone(&htim1, TIM_CHANNEL_2, 400, 30);
    		gameInstance.ship.updateShipHp(-1);
    		bossInstance.updateDisplayStatus(SHOULD_HIDE);
    		gameInstance.ship.updateCoordinate(104, 260);
    	}
    }

    // Boss bắn đạn
    if (bossInstance.updateBullet(dt)) {
        for (int i = 0; i < MAX_BULLET; i++) {
            if (enemyBullet[i].displayStatus != IS_HIDDEN) continue;
            enemyBullet[i].updateCoordinate(bossInstance.coordinateX + 16, bossInstance.coordinateY + 16);
            enemyBullet[i].updateDisplayStatus(SHOULD_SHOW);
            break;
        }
    }

}


//void updateBoss(uint8_t dt) {
//	static uint32_t bossSpawnTimer = 0;
//    bool allEnemiesHidden = true;
//    // Kiểm tra nếu tất cả kẻ địch đều ẩn
//    for (int i = 0; i < MAX_ENEMY; i++) {
//        if (enemy[i].displayStatus == IS_SHOWN) {
//            allEnemiesHidden = false;
//            bossSpawnTimer = 0; // Reset timer nếu có kẻ địch hiện
//            break;
//        }
//    }
//
//    // Tăng bộ đếm thời gian nếu tất cả kẻ địch đều ẩn
//    if (allEnemiesHidden) {
//        bossSpawnTimer += dt;
//    }
//
//    // Hiển thị boss nếu tất cả kẻ địch ẩn trong 5 giây
//    if (bossSpawnTimer >= 5000 && bossInstance.displayStatus == IS_HIDDEN) {
//        bossInstance.updateCoordinate(104, 40); // Tọa độ xuất hiện
//        bossInstance.updateVelocity(1, 0);      // Bắt đầu di chuyển sang phải
//        bossInstance.updateDisplayStatus(SHOULD_SHOW);
//        bossSpawnTimer = 0; // Reset timer sau khi boss xuất hiện
//    }
//
//    if (bossInstance.displayStatus != IS_SHOWN) return;
//
//    // Kiểm tra biên để thay đổi hướng di chuyển
//    const uint16_t leftBoundary = bossInstance.width;  // Giới hạn bên trái
//    const uint16_t rightBoundary = 240 - bossInstance.width; // Giới hạn bên phải
//
//    // Nếu boss chạm biên, đổi hướng
//    if (bossInstance.coordinateX <= leftBoundary && bossInstance.velocityX < 0) {
//        bossInstance.updateVelocity(1, 0); // Đổi hướng sang phải
//    } else if (bossInstance.coordinateX >= rightBoundary && bossInstance.velocityX > 0) {
//        bossInstance.updateVelocity(-1, 0); // Đổi hướng sang trái
//    }
//
//    // Cập nhật vị trí và trạng thái của boss
//    bossInstance.update(dt);
//
//    // Kiểm tra va chạm với đạn của người chơi
//    for (int i = 0; i < MAX_BULLET; i++) {
//        if (shipBullet[i].displayStatus != IS_SHOWN) continue;
//        if (Entity::isCollide(bossInstance, shipBullet[i])) {
//            bossInstance.updateBossHp(-1);
//            shipBullet[i].updateDisplayStatus(SHOULD_HIDE);
//
//            if (bossInstance.lives <= 0) {
//                bossInstance.updateDisplayStatus(SHOULD_HIDE);
//                gameInstance.updateScore(50); // Thưởng điểm khi tiêu diệt boss
//            }
//            break;
//        }
//    }
//
//    // Boss bắn đạn
//    if (bossInstance.updateBullet(dt)) {
//        for (int i = 0; i < MAX_BULLET; i++) {
//            if (enemyBullet[i].displayStatus != IS_HIDDEN) continue;
//            enemyBullet[i].updateCoordinate(bossInstance.coordinateX + 16, bossInstance.coordinateY + 16);
//            enemyBullet[i].updateDisplayStatus(SHOULD_SHOW);
//            break;
//        }
//    }
//}


void Intro(){
	#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
	#define C2 65
	#define C3 131
	#define Cb4 277
	#define Cb3 139

	const int notes[99][3] = {
	 {C2, 159, 96},
	 {C2, 65, 288},
	 {C2, 97, 383},
//	 {C3, 96, 352},
//	 {Cb4, 32, 320},
//	 {C2, 128, 96},
//	 {C2, 64, 289},
//	 {C2, 96, 384},
//	 {C2, 96, 352},
//	 {Cb4, 32, 289},
//	 {C2, 159, 96},
//	 {C2, 65, 288},
//	 {C2, 96, 384},
//	 {C3, 65, 416},
//	 {C2, 64, 289},
//	 {C2, 128, 96},
//	 {C2, 96, 256},
//	 {C2, 96, 384},
//	 {C3, 64, 416},
//	 {C2, 65, 287},
//	 {C2, 128, 96},
//	 {C2, 97, 255},
//	 {C2, 96, 384},
//	 {C3, 96, 384},
//	 {C2, 96, 256},
//	 {C3, 128, 96},
//	 {C3, 64, 256},
//	 {C3, 97, 352},
//	 {C3, 96, 383},
//	 {Cb4, 65, 287},
//	 {C3, 128, 96},
//	 {C3, 96, 224},
//	 {C3, 96, 352},
//	 {C3, 96, 384},
//	 {C2, 128, 0},
//	 {Cb3, 64, 160},
//	 {C3, 128, 96},
//	 {C3, 96, 224},
//	 {C3, 97, 352},
//	 {C3, 96, 383},
//	 {C3, 96, 32},
//	 {Cb3, 65, 159},
//	 {C3, 128, 96},
//	 {C3, 96, 224},
//	 {C3, 96, 352},
//	 {C3, 96, 384},
//	 {C2, 96, 256},
//	 {C3, 128, 96},
//	 {C3, 97, 224},
//	 {C3, 97, 351},
//	 {C3, 97, 383},
//	 {C2, 96, 256},
//	 {C3, 128, 96},
//	 {C3, 96, 224},
//	 {C3, 96, 352},
//	 {C3, 96, 384},
//	 {C2, 96, 256},
//	 {C3, 128, 127},
//	 {C3, 97, 255},
//	 {C3, 97, 351},
//	 {C3, 96, 352},
//	 {C2, 96, 704},
//	 {C3, 65, 0},
	};
	for (int i = 0; i < ARRAY_LEN(notes); i++) {
		tone(&htim1, TIM_CHANNEL_2, notes[i][0] , notes[i][1]);
		HAL_Delay(notes[i][2]);
	  }
}

void Outro(){
	#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
	#define Ab5 932
	#define Ab6 1865
	#define A6 1760
	#define B5 988
	#define Fb6 1480

	const int notes[5][3] = {
			 {Ab5, 34, 0},
			 {Ab6, 64, 64},
			 {A6, 34, 95},
			 {B5, 64, 0},
			 {Fb6, 95, 0},
	};
	for (int i = 0; i < ARRAY_LEN(notes); i++) {
			tone(&htim1, TIM_CHANNEL_2, notes[i][0] , notes[i][1]);
			HAL_Delay(notes[i][2]);
		  }
}

void noTone(TIM_HandleTypeDef *htim, uint32_t channel){
    HAL_TIM_PWM_Stop(htim, channel);
}

// Cập nhật hàm gameTask để xử lý boss
void gameTask(void *argument) {
    int loopCount = 0;

    for (int i = 0; i < MAX_BULLET; i++) {
        shipBullet[i].updateCoordinate(-50, -50);
        enemyBullet[i].updateCoordinate(-50, -50);
        shipBullet[i].updateVelocity(0, -5);
        enemyBullet[i].updateVelocity(0, 5);
        shipBullet[i].updateDisplayStatus(IS_HIDDEN);
        enemyBullet[i].updateDisplayStatus(IS_HIDDEN);
    }

    for (int i = 0; i < MAX_ENEMY; i++) {
        enemy[i].updateCoordinate(-50, -50);
        enemy[i].updateDisplayStatus(IS_HIDDEN);
    }

    resetGame();
//    tone(&htim1, TIM_CHANNEL_2, 400, 1000);
    Intro();

    for (;;) {
        loopCount++;
        if (shouldEndGame == true && shouldStopTask == true) {
            break;
        } else if (shouldEndGame) {
            continue;
        }

        if (loopCount >= 2) {
            dt = 1;
            loopCount = 0;
        } else {
            dt = 0;
        }

        sx = gameInstance.ship.coordinateX;
        sy = gameInstance.ship.coordinateY;

        updateEnemy(dt);
        updateBoss(dt); // Cập nhật boss
        gameInstance.ship.update(dt);
        updateShipBullet(dt);
        updateEnemyBullet(dt);

        for (int i = 0; i < MAX_ENEMY; i++) {
            if (enemy[i].displayStatus != IS_SHOWN) continue;
            if (Entity::isCollide(enemy[i], gameInstance.ship)) {
            	tone(&htim1, TIM_CHANNEL_2, 400, 30);
                gameInstance.ship.updateShipHp(-1);
                enemy[i].updateDisplayStatus(SHOULD_HIDE);
                gameInstance.ship.updateCoordinate(104, 260);
                break;
            }
        }

        for (int i = 0; i < MAX_BULLET; i++) {
            if (enemyBullet[i].displayStatus != IS_SHOWN) continue;
            if (Entity::isCollide(enemyBullet[i], gameInstance.ship)) {
            	tone(&htim1, TIM_CHANNEL_2, 400, 30);
                gameInstance.ship.updateShipHp(-1);
                enemyBullet[i].updateDisplayStatus(SHOULD_HIDE);
                gameInstance.ship.updateCoordinate(104, 260);
                break;
            }
        }

        for(int i=0; i<MAX_ENEMY; i++) {
			if(enemy[i].displayStatus != IS_SHOWN) continue;
			for(int j=0; j<MAX_BULLET; j++) {
				if(shipBullet[j].displayStatus != IS_SHOWN) continue;
				if(Entity::isCollide(enemy[i], shipBullet[j])) {
					tone(&htim1, TIM_CHANNEL_2, 400, 30);
					enemy[i].updateDisplayStatus(SHOULD_HIDE);
					shipBullet[j].updateDisplayStatus(SHOULD_HIDE);
					gameInstance.updateScore(10);
					break;
				}
			}
		}

        if (gameInstance.ship.lives <= 0) {
            shouldEndGame = true;
            gameInstance.updateHighScore(gameInstance.score);
            resetGame();
        }
    }
}

void updateEnemy(uint8_t dt) {
    spawnTimer -= dt; // Giảm thời gian chờ sinh kẻ địch
    if (spawnTimer > 0) return; // Nếu chưa đến thời điểm sinh, thoát khỏi hàm

    // Giới hạn số lượng wave kẻ địch tối đa là 4
    if (currentWave >= 4) return;

    // Tạo 1 wave mới nếu tất cả kẻ địch của wave hiện tại đã được sinh
    if (enemiesSpawnedInWave >= 4) { // Mỗi wave có 4 kẻ địch
        currentWave++;                     // Tăng wave hiện tại
        enemiesSpawnedInWave = 0;          // Reset số lượng kẻ địch trong wave
        waveOffsetY += 40;                 // Dịch xuống 1 hàng
        if (currentWave % 3 == 2) {
            waveOffsetX += 20;             // Wave 2 dịch phải
        } else if (currentWave % 3 == 0) {
            waveOffsetX -= 20;             // Wave 3 dịch trái
        }

        spawnTimer = 10000; // Thiết lập thời gian chờ cho wave tiếp theo
        return;
    }

    // Sinh từng kẻ địch trong wave
    for (int i = 0; i < MAX_ENEMY; i++) {
        if (enemy[i].displayStatus == IS_HIDDEN) { // Chỉ chọn slot trống
            // Tính toán tọa độ cố định cho kẻ địch
            const uint16_t startX = 10;              // Điểm bắt đầu cố định trên trục X
            const uint16_t startY = 40;             // Điểm bắt đầu cố định trên trục Y
            const uint16_t gapX = 50;               // Khoảng cách giữa các kẻ địch

            int col = enemiesSpawnedInWave;         // Chỉ số kẻ địch trong wave
            uint16_t ex = startX + col * gapX + waveOffsetX; // Tọa độ X
            uint16_t ey = startY + waveOffsetY;             // Tọa độ Y

            // Cập nhật thông tin cho kẻ địch
            enemy[i].updateCoordinate(ex, ey);
            enemy[i].updateVelocity(0, 1);          // Di chuyển dọc xuống
            enemy[i].updateDisplayStatus(SHOULD_SHOW);

            enemiesSpawnedInWave++;                // Tăng số lượng kẻ địch trong wave
            spawnTimer = 10000;       // Nghỉ ngắn trước khi sinh kẻ địch tiếp theo
            break;
        }
    }
}


void updateShipBullet(uint8_t dt) {
    bool shouldFire = gameInstance.ship.updateBullet(dt);
    for (int i = 0; i < MAX_BULLET; i++) {
        if (shipBullet[i].displayStatus != IS_HIDDEN) {
            shipBullet[i].update(dt);
            continue;
        }

        if (shouldFire) {
            shipBullet[i].updateCoordinate(sx + 16, sy - 16);
            shipBullet[i].updateDisplayStatus(SHOULD_SHOW);
            shouldFire = false;
        }
    }
}



void updateEnemyBullet(uint8_t dt) {
    int shootingEnemies = 0;
    for (int j = 0; j < MAX_ENEMY; j++) {
        if (enemy[j].displayStatus != IS_SHOWN) continue;
        if (enemy[j].updateBullet(dt) == false) continue;

        if (shootingEnemies >= 1) break;
        static uint32_t seed = 12345;
        for (int i = 0; i < MAX_BULLET; i++) {
        	seed = (1103515245 * seed + 12345) % 0x7FFFFFFF;
        	if(seed % 10){
				if (enemyBullet[i].displayStatus != IS_HIDDEN) continue;
				enemyBullet[i].updateCoordinate(enemy[j].coordinateX + 16, enemy[j].coordinateY + 16);
				enemyBullet[i].updateDisplayStatus(SHOULD_SHOW);
				if (shootingEnemies < 2) shootingEnemies++;
				break;
        	}
        }
    }

    for (int i = 0; i < MAX_BULLET; i++) {
        if (enemyBullet[i].displayStatus != IS_SHOWN) continue;
        enemyBullet[i].update(dt);
    }
}


