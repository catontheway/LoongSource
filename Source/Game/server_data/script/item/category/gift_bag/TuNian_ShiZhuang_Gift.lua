
--2011���´�ʱװ���

--6010832	����ʱװ��У�7�죩
--6010833	����ʱװ��У�30�죩
--6010834	����ʱװ��У����ã�

--7����ʱװ���

function I6010832_GiftBag(MapID, InstanceID, TypeID, RoleID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local Sex =  role.GetRoleSex(RoleID)
	if  Sex == 1 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143201, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143202, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143203, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143204, 1, 4, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143205, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143206, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143207, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143208, 1, 4, 8, 420)
	end
end

function I6010832_CanUseGiftBag(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(6010832, 1, "I6010832_GiftBag")
aux.RegisterItemEvent(6010832, 0, "I6010832_CanUseGiftBag")



--30��ʱװ���

function I6010833_GiftBag(MapID, InstanceID, TypeID, RoleID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local Sex =  role.GetRoleSex(RoleID)
	if  Sex == 1 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143209, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143210, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143211, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143212, 1, 4, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143213, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143214, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143215, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143216, 1, 4, 8, 420)
	end
end

function I6010833_CanUseGiftBag(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(6010833, 1, "I6010833_GiftBag")
aux.RegisterItemEvent(6010833, 0, "I6010833_CanUseGiftBag")

--����ʱװ���

function I6010834_GiftBag(MapID, InstanceID, TypeID, RoleID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local Sex =  role.GetRoleSex(RoleID)
	if  Sex == 1 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143217, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143218, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143219, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143220, 1, 4, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143221, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143222, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143223, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 9143224, 1, 4, 8, 420)
	end
end

function I6010834_CanUseGiftBag(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(6010834, 1, "I6010834_GiftBag")
aux.RegisterItemEvent(6010834, 0, "I6010834_CanUseGiftBag")































