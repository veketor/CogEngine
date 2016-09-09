﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Messages;

namespace CopterDown.Game
{
    public class ScoreB : ABehavior
    {
        public override void OnMessage(Messages.Message msg)
        {
            if (msg.Type == MessageType.GAMEOBJECT_DESTROYED)
            {
                var gameObj = msg.Data as GameObject;
                if (gameObj.GetObjectType() == ObjectType.COPTER)
                {
                    GameObject.GetSceneRoot().FindModelAtt<int>(AT.AT_COPTER_PLAYER_SCORE).Value += 100;
                }else if (gameObj.GetObjectType() == ObjectType.PARA)
                {
                    GameObject.GetSceneRoot().FindModelAtt<int>(AT.AT_COPTER_PLAYER_SCORE).Value += 20;
                }
                
            }
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            
        }
    }
}
