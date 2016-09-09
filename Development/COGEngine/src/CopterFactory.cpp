#include "CopterFactory.h"
#include "BeTranslateAnim.h"
#include "BeRotateAnim.h"
#include "BeMove.h"
#include "BeHitEvent.h"
#include "BeCollider.h"
#include "BeRender.h"
#include "SmartPointer.h"
#include "BeTween.h"

void OnBackgroundhit(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner){
	GNode* root = owner->GetRoot();
	GNode* scene = new GNode(ObjType::SCENE, 14, "scene");
	scene->AddBehavior(new BeRender(RenderType::IMAGE));
	spt<ofImage> img = MEngine.resourceCtrl->Get2DImage("images/background.png");
	scene->AddAttr(Attrs::IMGSOURCE, img);
	float scale = MEngine.environmentCtrl->GetWidth() / img->width;

	int width = MEngine.environmentCtrl->GetWidth();
	int height = MEngine.environmentCtrl->GetHeight();

	scene->GetTransform().LocalPos = Vectorf3(width/2+width, height/2);
	scene->GetTransform().Scale = scale;

	root->AddChild(scene);
	scene->AddBehavior(new BeSlideTween(TweenDirection::LEFT, scene, owner,1));
}


GNode* CopterFactory::CreateRoot(){

	GNode* root = new GNode(ObjType::ROOT, 12, "fof�k");
	GNode* splash = new GNode(ObjType::SCENE, 13, "sc�na");

	this->SetSingleBackground(splash, "images/intro.png", true);
	splash->AddBehavior(new BeHitEvent(OnBackgroundhit));

	root->AddChild(splash);
	return root;
}