SanDuXinMo_Boss_Num = 0				-- boss����ɱ�ĸ���
SanDuXinMo_Boss_ID = {}				-- �����ħ��ID��һСʱ����δ���ڽ�ս״̬��ɾ��boss

-- boss����
function x1523001_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	SanDuXinMo_Boss_Num = SanDuXinMo_Boss_Num + 1

	-- �����ħλ�ã����������丽��ˢ�³�һ����ħ����
	local x, y, z = unit.GetPosition(MapID, InstanceID, CreatureID)
	map.MapCreateCreature(MapID, InstanceID, 3020753, x+2, y, z+2)

	-- �����ħȫ������ɱ��ȫ������ҵõ�һ��buff
	if SanDuXinMo_Boss_Num >= 3 then
		SanDuXinMo_Boss_AllDie(MapID, InstanceID, 2016401)
	end
	Increase_shili(MapID, InstanceID, KillerID, 1, 20,"jinjieshili_A")

end

-- boss����
function x1523002_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	SanDuXinMo_Boss_Num = SanDuXinMo_Boss_Num + 1

	-- �����ħλ�ã����������丽��ˢ�³�һ����ħ����
	local x, y, z = unit.GetPosition(MapID, InstanceID, CreatureID)
	map.MapCreateCreature(MapID, InstanceID, 3020753, x+2, y, z+2)

	-- �����ħȫ������ɱ��ȫ������ҵõ�һ��buff
	if SanDuXinMo_Boss_Num >= 3 then
		SanDuXinMo_Boss_AllDie(MapID, InstanceID, 2016401)
	end
	Increase_shili(MapID, InstanceID, KillerID, 1, 20,"jinjieshili_A")


end

-- boss����
function x1523003_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, KillerID)
	SanDuXinMo_Boss_Num = SanDuXinMo_Boss_Num + 1

	-- �����ħλ�ã����������丽��ˢ�³�һ����ħ����
	local x, y, z = unit.GetPosition(MapID, InstanceID, CreatureID)
	map.MapCreateCreature(MapID, InstanceID, 3020753, x+2, y, z+2)

	-- �����ħȫ������ɱ��ȫ������ҵõ�һ��buff
	if SanDuXinMo_Boss_Num >= 3 then
		SanDuXinMo_Boss_AllDie(MapID, InstanceID, 2016401)
	end
	Increase_shili(MapID, InstanceID, KillerID, 1, 20,"jinjieshili_A")

end

-- ��ħȫ������
function SanDuXinMo_Boss_AllDie(MapID, InstanceID, BuffID)
	-- ��0
	SanDuXinMo_Boss_Num = 0

	-- �㲥��ҵõ�buff
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 232)
	msg.DispatchMapMsgEvent(MsgID, 3017299151, -1)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 232)
	msg.DispatchMapMsgEvent(MsgID, 1146339967, -1)

	map.AddMapRoleBuff(MapID, InstanceID, BuffID)

	-- �㲥������翪��
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 233)
	msg.DispatchMapMsgEvent(MsgID, 3017299151, -1)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 233)
	msg.DispatchMapMsgEvent(MsgID, 1146339967, -1)

	-- ˢ��npc
	map.MapCreateCreature(3017299151, -1, 3020752, 602, 7220, 409)
end

-- ���ʼ
function SanDuXinMo_OnStart(actID)
	-- ˢ��������ħ
	SanDuXinMo_Boss_ID[0] = map.MapCreateCreature(3017299151, -1, 1523001, 2538, 5000, 541)
	SanDuXinMo_Boss_ID[1] = map.MapCreateCreature(3017299151, -1, 1523002, 2646, 8130, 1227)
	SanDuXinMo_Boss_ID[2] = map.MapCreateCreature(3017299151, -1, 1523003, 1927, 3500, 2645)

	SanDuXinMo_Boss_Num = 0

	-- �㲥������ħ���ʼ
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 231)
	msg.DispatchMapMsgEvent(MsgID, 3017299151, -1)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 231)
	msg.DispatchMapMsgEvent(MsgID, 1146339967, -1)

end

-- �����
function SanDuXinMo_OnEnd(actID)
    local CanDelete = 1
	for i=0, 2 do
		if unit.IsAttack(3017299151, -1, SanDuXinMo_Boss_ID[i]) == 1 then
			CanDelete = 0
		end
	end

	-- ���boss���ڽ�ս״̬�У���ɾ��
	--[[if CanDelete == 1 then
		-- ɾ��boss
		map.MapDeleteCreature(3017299151, -1, SanDuXinMo_Boss_ID[0])
		map.MapDeleteCreature(3017299151, -1, SanDuXinMo_Boss_ID[1])
		map.MapDeleteCreature(3017299151, -1, SanDuXinMo_Boss_ID[2])
	end]]
end

-- �����޺�
function q14043_OnAccept(MapID, InstanceID, QuestID, OwnerID, NPCID)
	role.RoleGotoNewMap(MapID, InstanceID, OwnerID, 1826988583, 224, 4780, 857)
end
aux.RegisterQuestEvent(14043, 0,"q14043_OnAccept")

-- ��������
function q14043_OnComplete(MapID, InstanceID, QuestID, OwnerID, NPCID)
	role.RoleGotoNewMap(MapID, InstanceID, OwnerID, 3017299151, 602, 7220, 409)
end
aux.RegisterQuestEvent(14043, 1,"q14043_OnComplete")

-- ����ʹ�߶Ի����ͽ��븱��
function x3020752_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 1826988583, 438, 7389, 308)
		--��������С���У������������鷢�ͽ��븱��������
		role.InstanceNotify(RoleID, true)

	elseif TalkIndex == 5 then

	else

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 235)	-- "���Ƿ�Ҫ���뾻����磿"
		msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
		msg.AddMsgEvent(MsgID, 21, 5)	-- ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 207)	-- "ȡ��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	end
end


aux.RegisterCreatureEvent(3020752, 7, "x3020752_OnTalk")

aux.RegisterCreatureEvent(1523001, 4, "x1523001_OnDie")
aux.RegisterCreatureEvent(1523002, 4, "x1523002_OnDie")
aux.RegisterCreatureEvent(1523003, 4, "x1523003_OnDie")

aux.RegisterActEvent(44, 2, "SanDuXinMo_OnStart")
aux.RegisterActEvent(45, 2, "SanDuXinMo_OnStart")
aux.RegisterActEvent(46, 2, "SanDuXinMo_OnStart")
aux.RegisterActEvent(47, 2, "SanDuXinMo_OnStart")
aux.RegisterActEvent(44, 3, "SanDuXinMo_OnEnd")
aux.RegisterActEvent(45, 3, "SanDuXinMo_OnEnd")
aux.RegisterActEvent(46, 3, "SanDuXinMo_OnEnd")
aux.RegisterActEvent(47, 3, "SanDuXinMo_OnEnd")
