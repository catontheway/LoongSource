--��ʿ֮·(��)��13211
--����1���ڶ���Ĺ԰[m05: 1915, 331]����ʹ����ƷѪ��[2613108]����������߿���[2613109]

--��ƷѪ��ʹ���жϺ���
function i2613108_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false

	--�������ǲ���ʹ��
	if MapID ~= 3017298127 then
	    bRet = 43
	else
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
        if x<=1899 or x>=1931 or z<=315 or z>=347 then  --�����ڸ÷�Χ��ʹ��
            bRet = 43
        end
    end

    --����
	return bRet, bIgnore
end

--��ƷѪ��ʹ��Ч������
function i2613108_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	role.QuestAddRoleItem(MapID, InstanceID, TargetID, 13211, 2613109, 1, 0, 102)
end

--ע��
aux.RegisterItemEvent(2613108, 1, "i2613108_QuestUsable")
aux.RegisterItemEvent(2613108, 0, "i2613108_CanUse")
