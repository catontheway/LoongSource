

--�������ʱ����

function q20173_OnComplete(MapID, InstanceID, QuestID, TargetID, AccepterID)
	map.MapCreateColCreature(MapID, InstanceID, 1534104, 321, 6323, 324,1)---------------
end


--ɽ����Ȧ����
function q20174_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	 local k = math.random(1000)
	if k <= 50 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 3303002, 1, -1, 8, 420)
	--�������㲥
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 2125)
	msg.AddMsgEvent(MsgID, 2, RoleID)	--���������
	msg.DispatchWorldMsgEvent(MsgID)
	end

end

--ע��
aux.RegisterQuestEvent(20173, 1, "q20173_OnComplete")
aux.RegisterQuestEvent(20174, 1, "q20174_OnComplete")


function q20174_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 5) then
		--��ʾ��ұ����ռ䲻��
		return 0
	end
	return 1
end

aux.RegisterQuestEvent(20174, 5, "q20174_OnCheckComplete")
