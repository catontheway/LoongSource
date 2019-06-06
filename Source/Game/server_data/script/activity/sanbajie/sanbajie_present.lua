--���˸�Ů�ڻ

SanBa_exp = {}
SanBa_exp[1] = 85
SanBa_exp[2] = 100
SanBa_exp[3] = 140
SanBa_exp[4] = 235
SanBa_exp[5] = 300
SanBa_exp[6] = 405
SanBa_exp[7] = 545
SanBa_exp[8] = 670
SanBa_exp[9] = 920
SanBa_exp[10] = 1130
SanBa_exp[11] = 1475
SanBa_exp[12] = 1843
SanBa_exp[13] = 2231
SanBa_exp[14] = 2639
SanBa_exp[15] = 3067
SanBa_exp[16] = 3515
SanBa_exp[17] = 3983
SanBa_exp[18] = 4471
SanBa_exp[19] = 4979
SanBa_exp[20] = 5507
SanBa_exp[21] = 6055
SanBa_exp[22] = 6868
SanBa_exp[23] = 7924

--�����ʻ�
function I3303309_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 3303308, 1, -1, 8, 420)
	if SanbaFunvjie == 1 then
		role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 6010836, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6010836, 1, -1, 8, 420)
	end
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = math.floor(rolelevel/5 + 1)
	if k <= 23 then
        role.AddRoleExp(MapID, InstanceID, RoleID, SanBa_exp[k])
	end
end

function I3303309_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local FreeSizeRole = role.GetBagFreeSize(RoleID)
	if SanbaFunvjie == 0 then
		if(FreeSize < 1) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		end
	elseif SanbaFunvjie == 1 then
		if(FreeSize < 2) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		elseif FreeSizeRole < 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4409)   --������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		end
	end
end

aux.RegisterItemEvent(3303309, 1, "I3303309_OnUse")
aux.RegisterItemEvent(3303309, 0, "I3303309_CanUse")

--����Ǿޱ
function I3303310_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
    local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 3303308, 3, -1, 8, 420)
	if SanbaFunvjie == 1 then
		role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 6010836, 3, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6010836, 3, -1, 8, 420)
	end
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = math.floor(rolelevel/5 + 1)
	if k <= 23 then
        role.AddRoleExp(MapID, InstanceID, RoleID, SanBa_exp[k]*2)
	end
end

function I3303310_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local FreeSizeRole = role.GetBagFreeSize(RoleID)
	if SanbaFunvjie == 0 then
		if(FreeSize < 1) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		end
	elseif SanbaFunvjie == 1 then
		if(FreeSize < 2) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		elseif FreeSizeRole < 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4409)   --������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		end
	end
end

aux.RegisterItemEvent(3303310, 1, "I3303310_OnUse")
aux.RegisterItemEvent(3303310, 0, "I3303310_CanUse")

--�����¼�
function I3303311_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
    local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 3303308, 5, -1, 8, 420)
	if SanbaFunvjie == 1 then
		role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 6010836, 5, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6010836, 5, -1, 8, 420)
	end
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = math.floor(rolelevel/5 + 1)
	if k <= 23 then
        role.AddRoleExp(MapID, InstanceID, RoleID, SanBa_exp[k]*3)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 2417)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.AddMsgEvent(MsgID, 4, 3303311)
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)
end

function I3303311_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local FreeSizeRole = role.GetBagFreeSize(RoleID)
	if SanbaFunvjie == 0 then
		if(FreeSize < 1) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		end
	elseif SanbaFunvjie == 1 then
		if(FreeSize < 2) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		elseif FreeSizeRole < 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4409)   --������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		end
	end
end

aux.RegisterItemEvent(3303311, 1, "I3303311_OnUse")
aux.RegisterItemEvent(3303311, 0, "I3303311_CanUse")

--����õ��
function I3303312_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
    local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 3303308, 9, -1, 8, 420)if SanbaFunvjie == 1 then
		role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 6010836, 9, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6010836, 9, -1, 8, 420)
	end
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = math.floor(rolelevel/5 + 1)
	if k <= 23 then
        role.AddRoleExp(MapID, InstanceID, RoleID, SanBa_exp[k]*8)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 2411)          --###������������ƣ�һ��ǧ�𣬽�һ������õ���͸�***����������������ƣ���***����������������ƣ���Ȼ���ܣ�ͬʱ���9ö֪��ӡ��
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.DispatchWorldMsgEvent(MsgID)
end

function I3303312_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local FreeSizeRole = role.GetBagFreeSize(RoleID)
	if SanbaFunvjie == 0 then
		if(FreeSize < 1) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		end
	elseif SanbaFunvjie == 1 then
		if(FreeSize < 2) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		elseif FreeSizeRole < 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4409)   --������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		end
	end
end

aux.RegisterItemEvent(3303312, 1, "I3303312_OnUse")
aux.RegisterItemEvent(3303312, 0, "I3303312_CanUse")

--�λ��黨
function I3303313_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
    local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 3303308, 5, -1, 8, 420)
	if SanbaFunvjie == 1 then
		role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 6010836, 5, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6010836, 5, -1, 8, 420)
	end
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = math.floor(rolelevel/5 + 1)
	if k <= 23 then
        role.AddRoleExp(MapID, InstanceID, RoleID, SanBa_exp[k]*3)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 2417)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.AddMsgEvent(MsgID, 4, 3303313)
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)
end

function I3303313_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local sex = role.GetRoleSex(RoleID)
	local FreeSizeRole = role.GetBagFreeSize(RoleID)
	if SanbaFunvjie == 0 then
		if(FreeSize < 1) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		elseif sex == 0 then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2403)   --��Ʒ�޷��ͳ���ֻ��������ҿ���ʹ�ø���Ʒ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		end
	elseif SanbaFunvjie == 1 then
		if(FreeSize < 2) then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		elseif FreeSizeRole < 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4409)   --������ҵĿռ䲻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		elseif sex == 0 then
			--��ʾ��ұ����ռ䲻��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2403)   --��Ʒ�޷��ͳ���ֻ��������ҿ���ʹ�ø���Ʒ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			bRet = 32
			return bRet, bIgnore
		end
	end
end

aux.RegisterItemEvent(3303313, 1, "I3303313_OnUse")
aux.RegisterItemEvent(3303313, 0, "I3303313_CanUse")

--�������
function I3303316_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
    local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 3303315, 1, -1, 8, 420)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = math.floor(rolelevel/5 + 1)
	if k <= 23 then
        role.AddRoleExp(MapID, InstanceID, RoleID, SanBa_exp[k]*5)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 2419)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)
end

function I3303316_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		bRet = 32
		return bRet, bIgnore
	end
end

aux.RegisterItemEvent(3303316, 1, "I3303316_OnUse")
aux.RegisterItemEvent(3303316, 0, "I3303316_CanUse")
