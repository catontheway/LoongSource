--�ɷ�ʹ�ú���
function I1330020_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330020, 0, "I1330020_CanUse")

function I1330020_OnUse(MapID, InstanceID, TypeID, TargetID)
	local r = math.random(100)
	if r >84 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2220401, 1, -1, 8, 102)
	elseif r >37 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3300103, 10, -1, 8, 102)
	elseif r > 0 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 1320023, 1, -1, 8, 102)
	end
end

aux.RegisterItemEvent(1330020, 1, "I1330020_OnUse")
--���ʯ(20%)���ɽ������*10(45%)������ƣ��ֵ���ߣ�35%��
