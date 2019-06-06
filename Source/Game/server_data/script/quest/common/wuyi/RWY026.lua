--RWY026 ��֤��ɽ
--����1��������ڶ����ڣ���NPCڵ��Ի���ɺ�ˢ��5������ڵ���ͬ��
--m06��ͼ����6��14026 ڵ���ͬ�� 1007026 ��¼����

function n14026_OnNPCTalk(MapID, InstanceID, QuestID, RoleID, Npcid, Npctypeid)
	if Npctypeid == 3050019 then
		local a = map.GetMapScriptData(MapID,InstanceID,1,6)
		if a < 15 then
			local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
			map.MapCreateCreature(MapID, InstanceID, 1007026, x+4, y, z)
			map.MapCreateCreature(MapID, InstanceID, 1007026, x-4, y, z)
			map.MapCreateCreature(MapID, InstanceID, 1007026, x+2, y, z+2)

			map.SetMapScriptData(MapID,InstanceID,1,6,a+3)
		end
	end
end
aux.RegisterQuestEvent(14026, 6, "n14026_OnNPCTalk")

function q14026_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,6)
	if a == nil then
		a = 0
		map.SetMapScriptData(MapID,InstanceID,1,6,0)
	end
	if map.GetMapScriptData(MapID,InstanceID,1,6) >= 15 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 148)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end
aux.RegisterQuestEvent(14026, 4, "q14026_OnCheckAccept")

--����������ScriptData����6 ��ֵ��1
function c1007026_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,6) --����Data����1
	map.SetMapScriptData(MapID,InstanceID,1,6,a-1)
end
aux.RegisterCreatureEvent(1007026,4,"c1007026_OnDie")
