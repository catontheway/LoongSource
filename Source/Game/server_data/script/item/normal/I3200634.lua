--ʹ����Ԫ��ҩ3200634ȡ������9425701
--�ɷ�ʹ�ú���
function i3200634_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
        --�����������������ʹ��
	if MapID ~= 3084408783 then
	    bRet = 43
	end

	--����
	return bRet, bIgnore
end

--ʹ��Ч��
function i3200634_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       unit.CancelBuff(MapID, InstanceID, TargetID, 9425701)
end

--ע��
aux.RegisterItemEvent(3200634, 1, "i3200634_QuestUsable")
aux.RegisterItemEvent(3200634, 0, "i3200634_CanUse")

