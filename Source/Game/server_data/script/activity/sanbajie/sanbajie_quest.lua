--���˸�Ů�ڻ

--20191�������ʱ
function q20191_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local k = math.random(100)
	if k <= 50 and k >= 41 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 3303314, 1, 1, 8, 420)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 2418)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchWorldMsgEvent(MsgID)
	end
end

--ע��
aux.RegisterQuestEvent(20191, 1, "q20191_OnComplete")

--20192�������ʱ
function q20192_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local k = math.random(100)
	if k <= 50 and k >= 26 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 3303314, 1, 1, 8, 420)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 2418)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchWorldMsgEvent(MsgID)
	end
end

--ע��
aux.RegisterQuestEvent(20192, 1, "q20192_OnComplete")

--20193�������ʱ
function q20193_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local k = math.random(100)
	if k <= 50 and k >= 26 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 3303314, 1, 1, 8, 420)
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 2418)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchWorldMsgEvent(MsgID)
	end
end

--ע��
aux.RegisterQuestEvent(20193, 1, "q20193_OnComplete")

--20195�������ؽű�

function q20195_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
    local i=role.GetRoleAttValue(MapID, InstanceID, RoleID, 71)
    if i >= 40 then
		return 1
	else
	    --��ʾ���40��������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2406)  --����Ҫ�ﵽ40���ϵ����ݲ��ܽ�ȡ������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    return 0
	end
end

function q20195_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local i=role.GetRoleAttValue(MapID, InstanceID, RoleID, 71)
    if i >= 40 then
	    local FreeSize = role.GetBagFreeSize(RoleID)
		if i >= 200 then
		    if FreeSize < 2 then
			    local MsgID = msg.BeginMsgEvent()
		        msg.AddMsgEvent(MsgID, 26, 2407)  --����Ҫ��������������λ������ɴ�����
		        msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 0
			end
		else
		    if FreeSize < 1 then
			    local MsgID = msg.BeginMsgEvent()
		        msg.AddMsgEvent(MsgID, 26, 2408)  --����Ҫ����һ��������λ������ɴ�����
		        msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 0
			end
		end
		return 1
	else
	    --��ʾ���40��������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2409)  --����Ҫ�ﵽ40���ϵ����ݲ�����ɸ�����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    return 0
	end
end

function q20195_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
    local i = role.GetRoleAttValue(MapID, InstanceID, RoleID, 71)
	if i >= 40 and i <= 65 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 3303310, 1, 1, 8, 420)
	elseif i >= 66 and i <= 99 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 3303311, 1, 1, 8, 420)
	elseif i >= 100 and i <= 149 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 3303312, 1, 1, 8, 420)
	elseif i >= 150 and i <= 199 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 3303312, 2, 1, 8, 420)
	elseif i >= 200 then
        role.AddRoleItem(MapID, InstanceID, RoleID, 3303312, 2, 1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 3303314, 1, 1, 8, 420)
	end
end

function q20343_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local Funvjie_Nvwazhilu = role.GetRoleScriptData(RoleID,1,RoleDataType["Funvjie_Nvwazhilu"])
	local cTime = os.time()
	if Funvjie_Nvwazhilu > 0 then
		local s = os.difftime(cTime,Funvjie_Nvwazhilu)
		if s < 7200 then
			if 7200 - s >= 60 then
				s = math.floor((7200-s)/60)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 12801)  --��ʣ���ٷ��Ӳſ�����ȡ
				msg.AddMsgEvent(MsgID, 12, s)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 0
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 12802)  --��ʣ�������Ӳſ�����ȡ
				msg.AddMsgEvent(MsgID, 12, 7200-s)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 0
			end
		else
		return 1
		end
	else
		return 1
	end
end

function q20343_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local lTime = os.time()
	role.SetRoleScriptData(RoleID,1,RoleDataType["Funvjie_Nvwazhilu"],lTime)
end

--ע��
aux.RegisterQuestEvent(20195, 5, "q20195_OnCheckComplete")
aux.RegisterQuestEvent(20195, 1, "q20195_OnComplete")
aux.RegisterQuestEvent(20195, 4, "q20195_OnCheckAccept")
aux.RegisterQuestEvent(20343, 1, "q20343_OnComplete")
aux.RegisterQuestEvent(20343, 4, "q20343_OnCheckAccept")

