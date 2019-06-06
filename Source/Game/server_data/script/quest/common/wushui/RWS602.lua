--���壺13602
--����1�����������ˢ���ĸ��ٱ�[1006404]������Ϊ��
--[m05: 2315, 17592, 782]
--[m05: 2317, 17586, 787]
--[m05: 2317, 17586, 795]
--[m05: 2313, 17600, 800]

--�����ȡʱ
function x13602_OnAccept(MapID, InstanceID, QuestID, TargetID, AccepterID)
	--m05��ͼ����3��13602 �ĸ��ٱ�[1006404]��¼����
	local a = map.GetMapScriptData(MapID,InstanceID,1,3)
	if a == nil then
		a = 0
	end
	if a < 12 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1006404, 2315, 17592, 782)--�ٱ�
		map.MapCreateCreature(MapID, InstanceID, 1006404, 2317, 17586, 787)--�ٱ�
		map.MapCreateCreature(MapID, InstanceID, 1006404, 2317, 17586, 795)--�ٱ�
		map.MapCreateCreature(MapID, InstanceID, 1006404, 2313, 17600, 800)--�ٱ�
		map.SetMapScriptData(MapID,InstanceID,1,3,a+4)
	end
end

--ע��
aux.RegisterQuestEvent(13602, 0, "x13602_OnAccept")

function q13602_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if map.GetMapScriptData(MapID,InstanceID,1,3) >= 12 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 148)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end

aux.RegisterQuestEvent(13602, 4, "q13602_OnCheckAccept")

--����������ScriptData���� ��ֵ��1
function c1006404_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,3) --����Data����3
	map.SetMapScriptData(MapID,InstanceID,1,3,a-1)
end
aux.RegisterCreatureEvent(1006404,4,"c1006404_OnDie")
