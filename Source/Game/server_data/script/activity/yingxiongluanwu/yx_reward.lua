

--�ж�����Ƿ��û����,�������κ����
local function GetRoleReward(RoleID)
    for k=1, 10 do
        if RoleID == act.GetActScriptData(14, 1, k) then
            return k, 14
        end

        --[[if RoleID == act.GetActScriptData(15, 1, k) then
            return k, 15
        end

        if RoleID == act.GetActScriptData(16, 1, k) then
            return k, 16
        end

        if RoleID == act.GetActScriptData(17, 1, k) then
            return k , 17
        end]]
    end
    return 0, 0
end

function C4500103_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if -1 == TalkIndex then
        --������μ��˱��ε�Ӣ��������ȡ������������������������ȡ������

		local MsgID = msg.BeginMsgEvent()

		msg.AddMsgEvent(MsgID, 20,	167)
		msg.AddMsgEvent(MsgID, 24,	TargetID)	    --npcid
		msg.AddMsgEvent(MsgID, 21,	4)			    --ȷ��
		msg.AddMsgEvent(MsgID, 1,	21)
		msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
		msg.AddMsgEvent(MsgID, 1,	22)

		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return
    end

    --������ѡ��ȷ��
	if 4 == TalkIndex then
	    --�жϻ�Ƿ��Ѿ�����
	    local bStart = act.GetActIsStart(14)
	    if bStart then
	        local MsgID = msg.BeginMsgEvent()

			msg.AddMsgEvent(MsgID, 20,	171)

			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		    return
	    end

	    --�ж�����Ƿ�μӹ�������������
	    local PosIndex, Group = GetRoleReward(RoleID)

	    if 0 == PosIndex then
	        --û�п�����ȡ�Ľ�����
	        local MsgID = msg.BeginMsgEvent()

			msg.AddMsgEvent(MsgID, 20,	169)
		    msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
		    msg.AddMsgEvent(MsgID, 1,	22)

			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		    return
	    end

	    if 1 == PosIndex then
	        local FreeSize = role.GetBagFreeSize(RoleID)
	        if 2 > FreeSize then
	            --���ҿռ䲻��
	            local MsgID = msg.BeginMsgEvent()

				msg.AddMsgEvent(MsgID, 20,	168)

				msg.DispatchRoleMsgEvent(RoleID, MsgID)
		        return
	        end

	        if 14 == Group then
	            --��ð󶨵���2612621
	            role.AddRoleItem(MapID, InstanceID, RoleID, 2612621, 1, 0, 13, 1001)
	        end

	        --[[if 15 == Group then
	            --��ð󶨵���2612621
	            role.AddRoleItem(MapID, InstanceID, RoleID, 2612622, 1, 0, 13, 1001)
	        end

	        if 16 == Group then
	            --��ð󶨵���2612621
	            role.AddRoleItem(MapID, InstanceID, RoleID, 2612623, 1, 0, 13, 1001)
	        end

	        if 17 == Group then
	            --��ð󶨵���2612621
	            role.AddRoleItem(MapID, InstanceID, RoleID, 2612624, 1, 0, 13, 1001)
	        end]]

	        --����8������Ӣ����2612629
	        role.AddRoleItem(MapID, InstanceID, RoleID, 2612629, 10, 0, 13, 1001)
	        act.SetActScriptData(Group, 1, PosIndex, 0)
	        act.SaveActScriptData(Group)
	    end

	    --�����2��3��
	    if 2 <= PosIndex and 3 >= PosIndex then
	        local FreeSize = role.GetBagFreeSize(RoleID)
	        if 1 > FreeSize then
	            --���ҿռ䲻��
	            local MsgID = msg.BeginMsgEvent()

				msg.AddMsgEvent(MsgID, 20,	168)

				msg.DispatchRoleMsgEvent(RoleID, MsgID)
		        return
	        end

	        --����4������Ӣ����
	        role.AddRoleItem(MapID, InstanceID, RoleID, 2612629, 8, 0, 13, 1001)
	        act.SetActScriptData(Group, 1, PosIndex, 0)
	        act.SaveActScriptData(Group)
	    end

	    --�����4-6��
	    if 4 <= PosIndex and 6 >= PosIndex then
	        local FreeSize = role.GetBagFreeSize(RoleID)
	        if 1 > FreeSize then
	            --���ҿռ䲻��
	            local MsgID = msg.BeginMsgEvent()

				msg.AddMsgEvent(MsgID, 20,	168)

				msg.DispatchRoleMsgEvent(RoleID, MsgID)
		        return
	        end

	        --����2������Ӣ����
	        role.AddRoleItem(MapID, InstanceID, RoleID, 2612629, 4, 0, 13, 1001)
	        act.SetActScriptData(Group, 1, PosIndex, 0)
            act.SaveActScriptData(Group)
	    end

	    --�����7-10
	    if 7 <= PosIndex and 10 >= PosIndex then
	        local FreeSize = role.GetBagFreeSize(RoleID)
	        if 1 > FreeSize then
	            --���ҿռ䲻��
	            local MsgID = msg.BeginMsgEvent()

				msg.AddMsgEvent(MsgID, 20,	168)

				msg.DispatchRoleMsgEvent(RoleID, MsgID)
		        return
	        end

            --����1������Ӣ����
	        role.AddRoleItem(MapID, InstanceID, RoleID, 2612629, 2, 0, 13, 1001)
	        act.SetActScriptData(Group, 1, PosIndex, 0)
            act.SaveActScriptData(Group)
	    end

	    --�������Ľ��������úá�
	    local MsgID = msg.BeginMsgEvent()

		msg.AddMsgEvent(MsgID, 20,	170)

		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end

--ע��һ������Ի��¼�
aux.RegisterCreatureEvent(4500103, 7, "C4500103_OnTalk")
