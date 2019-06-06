




--��������4900201�Ի���ѡ��ȷ�����۳�1�����M01.��M01�ĺ���Ի������ͳ�M01���ص����衣
function n4900201_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
        if MapID == 3017298383 then
		    if Marriage_Available == 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 501) -- "���뻨��1��ǰ��濾����������һͬ������"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, 4) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 21) -- "ȷ��"
				msg.AddMsgEvent(MsgID, 21, 6)   --ȡ����ť
				msg.AddMsgEvent(MsgID, 1, 22)   --��ȡ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 499) -- "����ϵͳ�������ţ������ڴ���"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, 6) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 21) -- "ȷ��"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
        elseif MapID == 3017298895 then
		    local MsgID = msg.BeginMsgEvent()
            msg.AddMsgEvent(MsgID, 20, 502) -- "�����뿪濾�ط��ط�����"
            msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
            msg.AddMsgEvent(MsgID, 1, 21) -- "ȷ��"
            msg.AddMsgEvent(MsgID, 21, 6)   --ȡ����ť
            msg.AddMsgEvent(MsgID, 1, 22)   --��ȡ����
            msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
        end
    elseif TalkIndex == 4 then
        local money = role.GetRoleSilver(MapID, InstanceID, RoleID)
        if money < 10000 then
            local MsgID = msg.BeginMsgEvent()
            msg.AddMsgEvent(MsgID, 20, 503) -- "�����ϵĽ�Ǯ���㣬�޷�ǰ��濾�ء�"
            msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
        else
            role.DecRoleSilver(MapID, InstanceID, RoleID, 10000, 200)-- ϵͳ�۳���Ӧ��Ǯ
            role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017298895, 745, 58, 831)
        end
    elseif TalkIndex == 5 then
        role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017298383, 771, 10034, 2078)      --���ͷ���
    end
end

aux.RegisterCreatureEvent(4900201, 7, "n4900201_OnTalk")

--�����4900203�Ի�������ÿ����һ������֮Ӱ1361101��
function n4900203_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 504) -- "濾������������صúܺõ�����������Դ��������ϻ�������Ѻöȵĵ��ߡ����Խ�ɫÿ��ֻ����������ȡһ������֮Ӱ��ʹ�������Է��������ķ�λ����ϴ��϶�����ȡ������֮�������ܽ�������濾�ص��������ҳ������������ھ���ȡһ������֮Ӱ��"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, 4) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 21) -- "ȷ��"
		msg.AddMsgEvent(MsgID, 21, 5)   --ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 22)   --��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
    elseif TalkIndex == 4 then
        local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize < 1) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 505) -- "�������ҿռ䲻�㣬�޷���ȡ����Ʒ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
        else                   --ʱ���жϣ�ÿ����ȡһ��
			local LastTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["YueYaoZhiYing_Date"])
            local CurTime = tonumber(os.date("%j"))
			if CurTime == LastTime then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 506) -- "�������Ѿ���ȡ������֮Ӱ�ˣ�������������ȡ��"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			elseif CurTime ~= LastTime then
				local sex = role.GetRoleSex(RoleID)
				if sex == 1 then
				    local i = role.GetRoleItemNum(RoleID, 1361101)
					if i > 0 then
					    role.RemoveFromRole(MapID, InstanceID, RoleID, 1361101, i, 420)
					end
					role.AddRoleItem(MapID, InstanceID, RoleID, 1361101, 1, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["YueYaoZhiYing_Date"], CurTime)
				elseif sex == 0 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 507) -- "ֻ�����Խ�ɫ��������������ȡ����֮Ӱ."
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				end
            end
        end
    end
end

aux.RegisterCreatureEvent(4900203, 7, "n4900203_OnTalk")

--���϶�4900202�Ի�������ÿ����һ������֮��1361102��
function n4900202_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 508) -- "濾������������صúܺõ�����������Դ��������ϻ�������Ѻöȵĵ��ߡ�Ů�Խ�ɫÿ��ֻ����������ȡһ������֮��������֮�������������������ĵط�ʹ�ã�ʹ���������������ھ���ȡһ������֮����"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, 4) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 21) -- "ȷ��"
		msg.AddMsgEvent(MsgID, 21, 5)   --ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 22)   --��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
    elseif TalkIndex == 4 then
        local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize < 1) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 505) -- "�������ҿռ䲻�㣬�޷���ȡ����Ʒ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
        else                   --ʱ���жϣ�ÿ����ȡһ��
			local LastTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["YueYaoZhiJing_Date"])
            local CurTime = tonumber(os.date("%j"))
			if CurTime == LastTime then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 509) -- "�������Ѿ���ȡ������֮���ˣ�������������ȡ��"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			elseif CurTime ~= LastTime then
				local sex = role.GetRoleSex(RoleID)
				if sex == 0 then
				    local i = role.GetRoleItemNum(RoleID, 1361102)
					if i > 0 then
					    role.RemoveFromRole(MapID, InstanceID, RoleID, 1361102, i, 420)
					end
					role.AddRoleItem(MapID, InstanceID, RoleID, 1361102, 1, -1, 8, 420)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["YueYaoZhiJing_Date"], CurTime)
				elseif sex == 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 510) -- "ֻ��Ů�Խ�ɫ��������������ȡ����֮��."
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				end
			end
        end
    end
end

aux.RegisterCreatureEvent(4900202, 7, "n4900202_OnTalk")
