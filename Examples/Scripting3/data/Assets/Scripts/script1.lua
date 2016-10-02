ScriptExample = Behavior:Extend("ScriptExample")
ScriptExample.animate = false

function ScriptExample:OnInit()
  self:SubscribeForMessages(StrId("GREEN_ROTATION_COMPLETED"))
end

function ScriptExample:OnMessage(msg)
    if(msg:HasAction(StrId("GREEN_ROTATION_COMPLETED")))
	  then
	   self.animate = true
	end
end

function ScriptExample:Update(delta, absolute)
    if(self.animate)
	then
        rot = self.owner.transform.rotation
        self.owner.transform.rotation = self.owner.transform.rotation+10
	    if(rot > 360)
	    then
	      self.owner.transform.rotation = 0
		  self.animate = false
		  self:SendMessage(StrId("RED_ROTATION_COMPLETED"))
	   end
     end
end


-- create instance
scriptExampleInst = ScriptExample:New()

