--RWY237����ս
--����1����������ˢ�µ���4����������Ӷ�Ա��һ����������Ӷӳ��������Ҫȫ����ɱ��
--m41����

function x14237_OnAccept(MapID, InstanceID, QuestID, TargetID, AccepterID)

	--41��ͼ����10��14237 ��������Ӷ�Ա 1008006 ��¼����
	local a = map.GetMapScriptData(MapID,InstanceID,1,10)--��������Ӷ�Ա 1008006 ,��ͼ����10
	if a == nil then
		a = 0
	end
	if a < 10 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1008006, 2104, 20897, 404)
		map.SetMapScriptData(MapID,InstanceID,1,10,a+1)
	end

	--41��ͼ����10��14237 ��������Ӷӳ� 1008007 ��¼����
	local b = map.GetMapScriptData(MapID,InstanceID,1,11)--��������Ӷӳ�,��ͼ����11
	if b == nil then
		b = 0
	end
	if b < 10 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1008007, 2101, 20895 , 407)
		map.SetMapScriptData(MapID,InstanceID,1,11,b+1)
	end
end

aux.RegisterQuestEvent(14237, 0, "x14237_OnAccept")

function q14237_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if map.GetMapScriptData(MapID,InstanceID,1,10) >= 10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 148)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	elseif map.GetMapScriptData(MapID,InstanceID,1,11) >= 10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 148)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end

aux.RegisterQuestEvent(14237, 4, "q14237_OnCheckAccept")

--����������ScriptData���� ��ֵ��1
function c1008006_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
		cre.MonsterSay(MapID, InstanceID, TargetID, 40024)
	end
	local a = map.GetMapScriptData(MapID,InstanceID,1,10) --����Data����10
	if a < 1 then
	a = 1
	end
	map.SetMapScriptData(MapID,InstanceID,1,10,a-1)
end
aux.RegisterCreatureEvent(1008006,4,"c1008006_OnDie")

function c1008007_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
		cre.MonsterSay(MapID, InstanceID, TargetID, 40026)
	end
	local a = map.GetMapScriptData(MapID,InstanceID,1,11) --����Data����11
	if a < 1 then
	a = 1
	end
	map.SetMapScriptData(MapID,InstanceID,1,11,a-1)
end
aux.RegisterCreatureEvent(1008007,4,"c1008007_OnDie")
