--����Ӫ����ʿ

function m02_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	--�������Ƿ���Ӫ����ʿ������
	local bHaveQuest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 20001)
	if bHaveQuest then
		local TriggerID = {}
		TriggerID[1], TriggerID[2], TriggerID[3] = role.GetQuestScriptData(RoleID, 20001, 3, 1, 2, 3)

		--�õ���ҵ�ǰ����
		local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)

		--�����Ҳȵ�������
		if ObjID == TriggerID[1] then
			--ˢ�³����Ǳ�����ʿ
			local NPCID = map.MapCreateCreature(MapID, InstanceID, 3010224, x, y, z)
			role.SetQuestScriptData(RoleID, 20001, 4, 1, 0, 4, NPCID)
		end

		if ObjID == TriggerID[2] then
			--���ˢ��1��4ֻ�������
			local CreatureNum = math.random(1, 4)
			for i=1, CreatureNum do
				map.MapCreateCreature(MapID, InstanceID, 1003161, x, y, z)
			end
			role.SetQuestScriptData(RoleID, 20001, 2, 2, 0)
		end

		if ObjID == TriggerID[3] then
			--���ˢ��1��4ֻ�������
			local CreatureNum = math.random(1, 4)
			for i=1, CreatureNum do
				map.MapCreateCreature(MapID, InstanceID, 1003161, x, y, z)
			end
			role.SetQuestScriptData(RoleID, 20001, 2, 3, 0)
		end
	end
end


--ʹ�øõ��ߣ��ᵯ����Ϣ����ʾ���ǳ����ڵ�3������
function I2610160_UesItem(MapID, InstanceID, TypeID, TargetID)
	local TriggerID = {}
	TriggerID[1], TriggerID[2], TriggerID[3] = role.GetQuestScriptData(TargetID, 20001, 3, 1, 2, 3)
	if TriggerID[1] == nil then
		return
	else

		--��������������������ͻ���
		local MsgInfoID = msg.BeginMsgEvent()
		local Index = math.random(1, 3)
		msg.AddMsgEvent(MsgInfoID, 13, 0)
		msg.AddMsgEvent(MsgInfoID, 1, 17)
		msg.AddMsgEvent(MsgInfoID, 7, TriggerID[Index])
		for i = 1, 3 do
			if i~=Index then
				msg.AddMsgEvent(MsgInfoID, 7, TriggerID[i])
			end
		end
		msg.DispatchRoleMsgEvent(TargetID, MsgInfoID)
	end
end

--���Ӫ����ʿ����
function x20001_Oncomplete(MapID, InstanceID, QuestID, OwnerID, NPCID)
	local NPC_ID = role.GetQuestScriptData(OwnerID, 20001, 1, 4)
	--��ɺ�ɾ�����Ǳ�����ʿ
	map.MapDeleteCreature(MapID, InstanceID, NPC_ID)

	Increase_shili(MapID, InstanceID, OwnerID, 0, 1, "jinjieshili_A")

end

--aux.RegisterItemEvent(2610160, 1, "I2610160_UesItem")
--aux.RegisterMapEvent("m02", 6, "m02_OnEnterTrigger")
--aux.RegisterQuestEvent(20001, 1, "x20001_Oncomplete")
