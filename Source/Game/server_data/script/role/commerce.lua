-- ����


--�������
function Commerce_OnCompleteCommerce(MapID, InstanceID, RoleID, TaelProgress)
	
	--����������������ָ������������125%ʱ�����1������"���з�չͭ��"��ID��3300001
	--����������������ָ������������150%ʱ�����1������"���з�չ����" ��ID��3300002
	--����������������ָ������������175%ʱ�����1������"���з�չ����" ��ID��3300003

	if TaelProgress > 175 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3300003	, 1, -1, 3, 104)
	elseif TaelProgress > 150 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3300002	, 1, -1, 3, 104)		
	elseif TaelProgress > 125 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3300001	, 1, -1, 3, 104)
	end
end

aux.RegisterRoleEvent(23, "Commerce_OnCompleteCommerce")
