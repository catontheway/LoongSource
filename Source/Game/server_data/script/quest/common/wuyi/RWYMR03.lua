--RWYMR03����΢֪��
--����1����������[x,y,z]������̨[x,y,z]������̨[x,y,z]����תԴ[x,y,z]�ĵ�����ʹ����Ʒ[������][2614026],���õ�[�����ؼ�¼][2614027],[����̨��¼][2614028],[����̨��¼][2614029],[��תԴ��¼][2614030]
--�����ص�ͼ����objIDΪ5494(��ͼ���Ϊm06)
--����̨��ͼ����objIDΪ5496(��ͼ���Ϊm06)
--����̨��ͼ����objIDΪ7929(��ͼ���Ϊm06_1)
--��תԴ��ͼ����objIDΪ7930(��ͼ���Ϊm06_1)


function I2614026_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	local a = map.IsInArea(3017299151, InstanceID, RoleID, 5494)
	local b = map.IsInArea(3017299151, InstanceID, RoleID, 5496)
	local c = map.IsInArea(1146339967, InstanceID, RoleID, 7929)
	local d = map.IsInArea(1146339967, InstanceID, RoleID, 7930)
	if a ~= true and b ~= true and c ~= true and d ~= true then --����������������ʹ��
		bRet = 43
	end
	return bRet, bIgnore

end

function I2614026_QuestUsable(MapID, InstanceID, TypeID, RoleID)
	local a = map.IsInArea(3017299151, InstanceID, RoleID, 5494)
	local b = map.IsInArea(3017299151, InstanceID, RoleID, 5496)
	local c = map.IsInArea(1146339967, InstanceID, RoleID, 7929)
	local d = map.IsInArea(1146339967, InstanceID, RoleID, 7930)
	if a == true then
	role.QuestAddRoleItem(MapID, InstanceID, RoleID, 14303, 2614027, 1, -1, 101)
	elseif b == true then
	role.QuestAddRoleItem(MapID, InstanceID, RoleID, 14303, 2614028, 1, -1, 101)
	elseif c == true then
	role.QuestAddRoleItem(MapID, InstanceID, RoleID, 14303, 2614029, 1, -1, 101)
	elseif d == true then
	role.QuestAddRoleItem(MapID, InstanceID, RoleID, 14303, 2614030, 1, -1, 101)
	end
end

aux.RegisterItemEvent(2614026, 1, "I2614026_QuestUsable")
aux.RegisterItemEvent(2614026, 0, "I2614026_CanUse")
