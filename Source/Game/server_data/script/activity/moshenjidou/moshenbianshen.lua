function moshenbianshen(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    local Buff = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)  --ˢ��NPCʱ�����λ�ü�������Ӧ��ֵ
	local Side = 0
	local index = 0
	for k,v in pairs(Jidou.RoomInfo[InstanceID].BlueFlag.BianShen) do
	    if v == Buff then
		    Side = "Blue"
			index = k
			break
		end
	end
	for k,v in pairs(Jidou.RoomInfo[InstanceID].RedFlag.BianShen) do
	    if v == Buff then
		    Side = "Red"
			index = k
			break
		end
	end

	if TalkIndex == -1 then
	    if Side ~= 0 and Side ~= Jidou.RoomInfo[InstanceID].Role[RoleID].Side then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	12911)		--��ֻ��ѡ���Լ�һ���Ľ�ɫ
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif Side == 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	12912)		--ÿ��Ӣ��ֻ��ѡ����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local HeroIntroduce = {} --����ÿ��NPC��Ӧ�Ĺ���˵����ID_MSG���ֵ
			HeroIntroduce[4902106] =12913  --BuffID=1123501
			HeroIntroduce[4902107] =12914  --BuffID=1123601
			HeroIntroduce[4902108] =12915  --BuffID=1123701
			HeroIntroduce[4902109] =12920  --BuffID=1123201
			HeroIntroduce[4902110] =12916  --BuffID=1123301
			HeroIntroduce[4902111] =12917  --BuffID=1123401
			HeroIntroduce[4902131] =12919  --BuffID=1123801
			HeroIntroduce[4902132] =12918  --BuffID=1123101
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	HeroIntroduce[TargetTypeID])		--��ѡ��
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	3721)			--��ѡ��
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 4 then
        local bool = true
		for i=11231, 11238 do
			if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, i) then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	3722)		--��ѡ��
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	22)			--�����鱨����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bool = false
				break
			end
		end

        if bool and Side == Jidou.RoomInfo[InstanceID].Role[RoleID].Side then --ʹ��ұ��β����ĵ�һ�����λ���
		    unit.CancelBuff(MapID, InstanceID, RoleID, 1121301)
			unit.AddBuff(MapID, InstanceID, RoleID, Buff, RoleID)
			unit.AddBuff(MapID, InstanceID, RoleID, 1141101, RoleID)
			if Side == "Red" then
				table.remove(Jidou.RoomInfo[InstanceID].RedFlag.BianShen,index)
			else
			    table.remove(Jidou.RoomInfo[InstanceID].BlueFlag.BianShen,index)
			end
		end
	end
end

aux.RegisterCreatureEvent(4902106, 7, "moshenbianshen")
aux.RegisterCreatureEvent(4902107, 7, "moshenbianshen")
aux.RegisterCreatureEvent(4902108, 7, "moshenbianshen")
aux.RegisterCreatureEvent(4902109, 7, "moshenbianshen")
aux.RegisterCreatureEvent(4902110, 7, "moshenbianshen")
aux.RegisterCreatureEvent(4902111, 7, "moshenbianshen")
aux.RegisterCreatureEvent(4902131, 7, "moshenbianshen")
aux.RegisterCreatureEvent(4902132, 7, "moshenbianshen")

