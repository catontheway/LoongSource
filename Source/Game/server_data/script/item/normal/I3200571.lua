--ʹ������¯��3200571�������ͽ������þ���1826990375��
--�ɷ�ʹ�ú���
function i3200571_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if MapID ~= 3017298383 or x < 652 or x > 1122 or z < 1872 or z > 2334 then
		return 43,false
	end
	return bRet, bIgnore
end

--ʹ��Ч��
function i3200571_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       role.RoleGotoNewMap(MapID, InstanceID, TargetID, 1826990375, 171, 5, 32)
end

--ע��
aux.RegisterItemEvent(3200571, 1, "i3200571_QuestUsable")
aux.RegisterItemEvent(3200571, 0, "i3200571_CanUse")


