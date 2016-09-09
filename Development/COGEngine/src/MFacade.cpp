
#include "MFacade.h"
#include "MGameEngine.h"
#include "MLogger.h"
#include "MGameStorage.h"

void COGLogError(const char* format, ...){
	va_list args;
	va_start(args, format);
	MEngine.logger->LogError(0,format, args);
	va_end(args);
}

void COGLogInfo(const char* format, ...){
	va_list args;
	va_start(args, format);
	MEngine.logger->LogInfo(0,format, args);
	va_end(args);
}

void COGLogDebug(const char* format, ...){
	va_list args;
	va_start(args, format);
	MEngine.logger->LogDebug(0,format, args);
	va_end(args);
}

void COGLoggerFlush(){
	MEngine.logger->Flush();
}

vector<EnInputAct>& COGGetPressedKeys(){
	return MEngine.environmentCtrl->GetPressedKeys();
}

vector<EnInputAct>& COGGetPressedPoints(){
	return MEngine.environmentCtrl->GetPressedPoints();
}

int COGGetWidth(){
	return MEngine.environmentCtrl->GetWidth();
}

int COGGetHeight(){
	return MEngine.environmentCtrl->GetHeight();
}

spt<ofImage> COGGet2DImage(string name){
	return MEngine.resourceCtrl->Get2DImage(name);
}

spt<ofVboMesh> COGGetMesh(string name){
	return MEngine.resourceCtrl->GetMesh(name);
}

spt<ofTrueTypeFont> COGGetFont(string name, int size){
	return MEngine.resourceCtrl->GetFont(name, size);
}


int COGRegisterCallback(int action, MsgCallback callback){
	return MEngine.storage->RegisterCallback(action, callback);
}

bool COGUnregisterCallback(int action, int id){
	return MEngine.storage->UnregisterCallback(action, id);
}

void COGRegisterListener(int action, GBehavior* beh){
	MEngine.storage->RegisterListener(action, beh);
}

void COGUnregisterListener(int action, GBehavior* beh){
	MEngine.storage->UnregisterListener(action, beh);
}

void COGSendTraversationMessage(GMsg& msg, GNode* actualNode){
	COGLogDebug("Message %s:%s:%d", Actions::ToString(msg.GetAction()), actualNode->GetTag().c_str(), actualNode->GetSubType());
	MEngine.storage->SendTraversationMessage(msg, actualNode);
}

void COGSendMessage(GMsg& msg){
	MEngine.storage->SendMessage(msg);
}


GNode* COGFindGameObjectById(int id) {
	return MEngine.storage->FindGameObjectById(id);
}

int COGGetGameObjectsCountByTag(string tag) {
	return MEngine.storage->GetGameObjectsCountByTag(tag);
}

GNode* COGFindGameObjectByTag(string tag) {
	return MEngine.storage->FindGameObjectByTag(tag);
}

vector<GNode*> COGFindGameObjectsByTag(char* tag) {
	return MEngine.storage->FindGameObjectsByTag(tag);
}

int COGGetGameObjectsCountBySubType(int subtype) {
	return MEngine.storage->GetGameObjectsCountBySubType(subtype);
}

GNode* COGFindGameObjectBySubType(int subtype) {
	return MEngine.storage->FindGameObjectBySubType(subtype);
}

vector<GNode*> COGFindGameObjectsBySubType(int subtype) {
	return MEngine.storage->FindGameObjectsBySubType(subtype);
}

bool COGAddGameObject(GNode* gameObject){
	COGLogDebug("Adding gameobject %s", gameObject->GetTag().c_str());
	return MEngine.storage->AddGameObject(gameObject);
}

void COGRemoveGameObject(GNode* gameObject){
	COGLogDebug("Removing gameobject %s", gameObject->GetTag().c_str());
	MEngine.storage->RemoveGameObject(gameObject);
}

bool COGAddBehavior(GBehavior* beh){
	return MEngine.storage->AddBehavior(beh);
}

void COGRemoveBehavior(GBehavior* beh){
	MEngine.storage->RemoveBehavior(beh);
}

float COGTranslateSpeed(float speed){
	return MEngine.TranslateSpeed(speed);
}