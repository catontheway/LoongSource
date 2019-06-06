--[[�����Ѳ⣺13233
--����1����ȡ�����Ժ���NPC���������ˢ��2����ç������[1006207]��һ����ç����[1006209]������Ϊ��
--[m02: 1949, 4500, 600]
--[m02: 1954, 4500, 585]
--[m02: 1943, 4500, 588]

--�����ȡʱ
function x13233_OnAccept(MapID, InstanceID, QuestID, TargetID, AccepterID)

	--m02��ͼ����5��13233 ����ç������[1006207]��¼����
	local a = map.GetMapScriptData(MapID,InstanceID,1,5)
	if a == nil then
		a = 0
	end
	if a < 10 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1006207, 1954, 4500, 585)--��ç������
		map.MapCreateCreature(MapID, InstanceID, 1006207, 1943, 4500, 588)--��ç������
		map.SetMapScriptData(MapID,InstanceID,1,5,a+2)
	end

	--m02��ͼ����6��13233 ��ç����[1006209]��¼����
	local b = map.GetMapScriptData(MapID,InstanceID,1,6)
	if b == nil then
		b = 0
	end
	if b < 5 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1006209, 1949, 4500, 600)
		map.SetMapScriptData(MapID,InstanceID,1,6,b+1)
	end

end

--ע��
aux.RegisterQuestEvent(13233, 0, "x13233_OnAccept")

function q13233_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if map.GetMapScriptData(MapID,InstanceID,1,5) >= 10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 148)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	elseif map.GetMapScriptData(MapID,InstanceID,1,6) >= 5 then
		local MsgID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID1, 13, 0)
		msg.AddMsgEvent(MsgID1, 1, 148)
		msg.DispatchRoleMsgEvent(RoleID, MsgID1)
		return 0
	end
	return 1
end

aux.RegisterQuestEvent(13233, 4, "q13233_OnCheckAccept")



--����������ScriptData���� ��ֵ��1

function c1006207_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,5) --����Data����5
		if a < 1 or a > 50 then
			a = 1
		end
	map.SetMapScriptData(MapID,InstanceID,1,5,a-1)
end
aux.RegisterCreatureEvent(1006207,4,"c1006207_OnDie")
--���ﺰ���ű�����д��������
function c1006209_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    cre.MonsterSay(MapID, InstanceID, TargetID, 30011)
	local a = map.GetMapScriptData(MapID,InstanceID,1,6) --����Data����6
		if a < 1 or a > 50 then
			a = 1
		end
	map.SetMapScriptData(MapID,InstanceID,1,6,a-1)
end
aux.RegisterCreatureEvent(1006209,4,"c1006209_OnDie")
]]
