--RKL151
--����1��ʹ�þ��޹Ǻ�2615051��������ĩ2615052

--�ɷ�ʹ�ú���
function i2615051_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	--����������أ�����ʹ��
	if MapID ~= 3017299407 then
	    bRet = 43
	else
	--�������ָ���ص㣬����ʹ��
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x < 2292 or x > 2392 or z < 1443 or z > 1543 then
	          bRet = 43 
            end
	end
    return bRet, bIgnore
end

--ʹ��Ч��
function i2615051_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       role.QuestAddRoleItem(MapID, InstanceID, TargetID, 15151, 2615052, 1, 0, 102)
end

--ע��
aux.RegisterItemEvent(2615051, 1, "i2615051_QuestUsable")
aux.RegisterItemEvent(2615051, 0, "i2615051_CanUse")