
--[[
��������ǿ�����

6010813	����ʯ��Ҽ��
6010814	����ʯ������
6010815	����ʯ������
6010816	����ʯ������
6010817	����ʯ���鼶
6010818	��ͨ����ʯ�ڼ�
6010819	�ϵ�����ʯ�ڼ�
6010820	��������ʯ�ڼ�
6010821	��Ʒ����ʯ�ڼ�

6010822	��������ʯ��Ҽ��
6010823	��������ʯ������
6010824	��������ʯ������
6010825	��������ʯ������
6010826	��������ʯ���鼶
6010827	��ͨ����ʯ�����ڼ�
6010828	�ϵ�����ʯ�����ڼ�
6010829	��������ʯ�����ڼ�
6010830	��Ʒ����ʯ�����ڼ�
]]

--ʹ���ж�
function NiePanShi_canUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	local FreeSize = role.GetBagFreeSize(RoleID)	--�жϱ������пռ��Ƿ��㹻
	if (FreeSize < 1) then
		bRet = 40	--��ʾ��ұ����ռ䲻��
		return bRet, bIgnore
	else
		--����ʯ�ڼ�
		if  TypeID == 6010818 then
			local Num_1 = 0
			local Num_2 = 0
			Num_1 = role.GetRoleItemNum(RoleID,6010813)
			Num_2 = role.GetRoleItemNum(RoleID,6010822)
			if Num_1 < 5 and Num_2 < 5 then
				bRet = 32	--��Ʒ����ʹ��
				return bRet, bIgnore
			else
				return bRet, bIgnore
			end
		elseif  TypeID == 6010819 then
			local Num_1 = 0
			local Num_2 = 0
			Num_1 = role.GetRoleItemNum(RoleID,6010814)
			Num_2 = role.GetRoleItemNum(RoleID,6010823)
			if Num_1 < 5 and Num_2 < 5 then
				bRet = 32	--��Ʒ����ʹ��
				return bRet, bIgnore
			else
				return bRet, bIgnore
			end
		elseif  TypeID == 6010820 then
			local Num_1 = 0
			local Num_2 = 0
			Num_1 = role.GetRoleItemNum(RoleID,6010815)
			Num_2 = role.GetRoleItemNum(RoleID,6010824)
			if Num_1 < 5 and Num_2 < 5 then
				bRet = 32	--��Ʒ����ʹ��
				return bRet, bIgnore
			else
				return bRet, bIgnore
			end
		elseif  TypeID == 6010821 then
			local Num_1 = 0
			local Num_2 = 0
			Num_1 = role.GetRoleItemNum(RoleID,6010816)
			Num_2 = role.GetRoleItemNum(RoleID,6010825)
			if Num_1 < 5 and Num_2 < 5 then
				bRet = 32	--��Ʒ����ʹ��
				return bRet, bIgnore
			else
				return bRet, bIgnore
			end
			--����ʯ�����ڼ�
		elseif  TypeID == 6010827 then
			local Num_1 = 0
			local Num_2 = 0
			Num_1 = role.GetRoleItemNum(RoleID,6010813)
			Num_2 = role.GetRoleItemNum(RoleID,6010822)
			if Num_1 < 5 and Num_2 < 5 then
				bRet = 32	--��Ʒ����ʹ��
				return bRet, bIgnore
			else
				return bRet, bIgnore
			end
		elseif  TypeID == 6010828 then
			local Num_1 = 0
			local Num_2 = 0
			Num_1 = role.GetRoleItemNum(RoleID,6010814)
			Num_2 = role.GetRoleItemNum(RoleID,6010823)
			if Num_1 < 5 and Num_2 < 5 then
				bRet = 32	--��Ʒ����ʹ��
				return bRet, bIgnore
			else
				return bRet, bIgnore
			end
		elseif  TypeID == 6010829 then
			local Num_1 = 0
			local Num_2 = 0
			Num_1 = role.GetRoleItemNum(RoleID,6010815)
			Num_2 = role.GetRoleItemNum(RoleID,6010824)
			if Num_1 < 5 and Num_2 < 5 then
				bRet = 32	--��Ʒ����ʹ��
				return bRet, bIgnore
			else
				return bRet, bIgnore
			end
		elseif  TypeID == 6010830 then
			local Num_1 = 0
			local Num_2 = 0
			Num_1 = role.GetRoleItemNum(RoleID,6010816)
			Num_2 = role.GetRoleItemNum(RoleID,6010825)
			if Num_1 < 5 and Num_2 < 5 then
				bRet = 32	--��Ʒ����ʹ��
				return bRet, bIgnore
			else
				return bRet, bIgnore
			end

		end
	end
end
aux.RegisterItemEvent(6010818, 0, "NiePanShi_canUse")
aux.RegisterItemEvent(6010819, 0, "NiePanShi_canUse")
aux.RegisterItemEvent(6010820, 0, "NiePanShi_canUse")
aux.RegisterItemEvent(6010821, 0, "NiePanShi_canUse")
aux.RegisterItemEvent(6010827, 0, "NiePanShi_canUse")
aux.RegisterItemEvent(6010828, 0, "NiePanShi_canUse")
aux.RegisterItemEvent(6010829, 0, "NiePanShi_canUse")
aux.RegisterItemEvent(6010830, 0, "NiePanShi_canUse")

--ʹ�õ���

function NiePanShi_OnUse(MapID, InstanceID, TypeID, RoleID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	--����ʯ�ڼ�
	if  TypeID == 6010818 then
		local Num_1 = 0
		local Num_2 = 0
		Num_1 = role.GetRoleItemNum(RoleID,6010813)
		Num_2 = role.GetRoleItemNum(RoleID,6010822)
		if Num_1 >= 5 then
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010813,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010814, 1, -1, 8, 420)
		else
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010822,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010814, 1, -1, 8, 420)
		end
	elseif  TypeID == 6010819 then
		local Num_1 = 0
		local Num_2 = 0
		Num_1 = role.GetRoleItemNum(RoleID,6010814)
		Num_2 = role.GetRoleItemNum(RoleID,6010823)
		if Num_1 >= 5 then
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010814,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010815, 1, -1, 8, 420)
		else
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010823,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010815, 1, -1, 8, 420)
		end
	elseif  TypeID == 6010820 then
		local Num_1 = 0
		local Num_2 = 0
		Num_1 = role.GetRoleItemNum(RoleID,6010815)
		Num_2 = role.GetRoleItemNum(RoleID,6010824)
		if Num_1 >= 5 then
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010815,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010816, 1, -1, 8, 420)
		else
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010824,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010816, 1, -1, 8, 420)
		end
	elseif  TypeID == 6010821 then
		local Num_1 = 0
		local Num_2 = 0
		Num_1 = role.GetRoleItemNum(RoleID,6010816)
		Num_2 = role.GetRoleItemNum(RoleID,6010825)
		if Num_1 >= 5 then
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010816,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010817, 1, -1, 8, 420)
		else
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010825,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010817, 1, -1, 8, 420)
		end
		--����ʯ�����ڼ�
	elseif  TypeID == 6010827 then
		local Num_1 = 0
		local Num_2 = 0
		Num_1 = role.GetRoleItemNum(RoleID,6010813)
		Num_2 = role.GetRoleItemNum(RoleID,6010822)
		if Num_1 >= 5 then
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010813,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010823, 1, -1, 8, 420)
		else
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010822,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010823, 1, -1, 8, 420)
		end
	elseif  TypeID == 6010828 then
		local Num_1 = 0
		local Num_2 = 0
		Num_1 = role.GetRoleItemNum(RoleID,6010814)
		Num_2 = role.GetRoleItemNum(RoleID,6010823)
		if Num_1 >= 5 then
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010814,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010824, 1, -1, 8, 420)
		else
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010823,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010824, 1, -1, 8, 420)
		end
	elseif  TypeID == 6010829 then
		local Num_1 = 0
		local Num_2 = 0
		Num_1 = role.GetRoleItemNum(RoleID,6010815)
		Num_2 = role.GetRoleItemNum(RoleID,6010824)
		if Num_1 >= 5 then
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010815,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010825, 1, -1, 8, 420)
		else
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010824,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010825, 1, -1, 8, 420)
		end
	elseif  TypeID == 6010830 then
		local Num_1 = 0
		local Num_2 = 0
		Num_1 = role.GetRoleItemNum(RoleID,6010816)
		Num_2 = role.GetRoleItemNum(RoleID,6010825)
		if Num_1 >= 5 then
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010816,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010826, 1, -1, 8, 420)
		else
			role.RemoveFromRole(MapID,InstanceID,RoleID,6010825,5,420)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6010826, 1, -1, 8, 420)
		end
	end
end
aux.RegisterItemEvent(6010818, 1, "NiePanShi_OnUse")
aux.RegisterItemEvent(6010819, 1, "NiePanShi_OnUse")
aux.RegisterItemEvent(6010820, 1, "NiePanShi_OnUse")
aux.RegisterItemEvent(6010821, 1, "NiePanShi_OnUse")
aux.RegisterItemEvent(6010827, 1, "NiePanShi_OnUse")
aux.RegisterItemEvent(6010828, 1, "NiePanShi_OnUse")
aux.RegisterItemEvent(6010829, 1, "NiePanShi_OnUse")
aux.RegisterItemEvent(6010830, 1, "NiePanShi_OnUse")




































