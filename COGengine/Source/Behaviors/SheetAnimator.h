#pragma once

#include "Behavior.h"
#include "AnimLoader.h"
#include "AnimContextStack.h"

namespace Cog {

	/**x
	* Behavior for common animations
	*/
	class SheetAnimator : public Behavior {
		OBJECT_PROTOTYPE_INIT(SheetAnimator)
	private:
		AnimContextStack contextStack;
	public:

		SheetAnimator(spt<SheetAnim> anim) {
			contextStack = AnimContextStack(anim);
		}

		SheetAnimator(Setting& setting) {
			string animation = setting.GetItemVal("animation");
			string renderTypeStr = setting.GetItemVal("render_type");

			auto resCache = GETCOMPONENT(ResourceCache);
			contextStack.SetRootNode(resCache->GetAnimation(animation));
		}

		void Init() {
			if (!contextStack.GetRootNode()) {
				CogLogError("Anim", "Animation cant' run, entity is null");
				ended = true;
				return;
			}
		}

		virtual void Update(const uint64 delta, const uint64 absolute) {

			this->contextStack.MoveToNext(delta);

			if (this->contextStack.Ended()) {
				Finish();
			}
			else {
				int actualIndex = (int)contextStack.GetContext().actualProgress;
				spt<SheetAnim> actualNode = contextStack.GetContext().GetActualChild();

				if (actualNode->GetFrames() > 1 || actualNode->GetLines() > 1) {
					// image is a spritesheet
					string imagePath = actualNode->GetSheet(0);
					spt<ofImage> spriteSheet = CogGet2DImage(imagePath);

					// calculate image offset
					int frameIndex = actualIndex + actualNode->GetStart();

					if (owner->HasRenderType(RenderType::SPRITE)) {
						// sprites

						// todo: performance problem because of recalculation
						auto spriteSet = owner->GetShape<spt<SpriteShape>>()->GetSprite()->GetSpriteSet();
						owner->GetShape<spt<SpriteShape>>()->SetSprite(spt<Sprite>(new Sprite(spriteSet, frameIndex)));
					}
					else {
						if (!owner->HasRenderType(RenderType::IMAGE)) {
							owner->SetShape(spt<Image>(new Image(spriteSheet)));
						}

						// images
						int frameRow = frameIndex / actualNode->GetFrames();
						int frameColumn = frameIndex % actualNode->GetFrames();
						int cellWidth = (int)(spriteSheet->getWidth() / actualNode->GetFrames());
						int cellHeight = (int)(spriteSheet->getHeight() / actualNode->GetLines());

						ofRectangle imageBound((float)(frameColumn*cellWidth), (float)(frameRow*cellHeight), (float)cellWidth, (float)cellHeight);
						owner->ChangeAttr(ATTR_IMGBOUNDS, imageBound);
						owner->GetShape<spt<Image>>()->SetImage(spriteSheet);

						if (owner->HasRenderType(RenderType::IMAGE)) {
							owner->GetShape<spt<Image>>()->SetImage(spriteSheet);
						}
						else if (owner->HasRenderType(RenderType::NONE)) {
							// set the first image
							owner->SetShape(spt<Image>(new Image(spriteSheet)));
						}
					}
				}
				else {
					// image is only a common image
					if (owner->HasAttr(ATTR_IMGBOUNDS)) owner->RemoveAttr(ATTR_IMGBOUNDS, true);

					string imagePath = actualNode->GetSheet(actualIndex);
					spt<ofImage> image = CogGet2DImage(imagePath);
					if (owner->HasRenderType(RenderType::IMAGE)) {
						owner->GetShape<spt<Image>>()->SetImage(image);
					}
					else {
						owner->SetShape(spt<Image>(new Image(image)));
					}
				}
			}
		}
	};

}// namespace