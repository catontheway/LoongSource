--���֮�ƣ�12245
--����1����Ӫ����[m04: 2587, 1861]����ʹ����Ʒα���ż�[2612133]����������߿��ŷ�[2612136]

--��Ʒα���½�ʹ���жϺ���
function i2612133_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	
	--������¹������ʹ��
	if MapID ~= 3017299663 then
	    bRet = 43
	else	
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x<=2571 or x>=2603 or z<=1845 or z>=1877 then    --�����ڸ÷�Χ��ʹ��
		    bRet = 43
	    end
	end

    --����
	return bRet, bIgnore
end

--��Ʒα���ż�ʹ��Ч������
function i2612133_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	role.QuestAddRoleItem(MapID, InstanceID, TargetID, 12245, 2612136, 1, 0, 102)
end

--ע��
aux.RegisterItemEvent(2612133, 1, "i2612133_QuestUsable")
aux.RegisterItemEvent(2612133, 0, "i2612133_CanUse")
