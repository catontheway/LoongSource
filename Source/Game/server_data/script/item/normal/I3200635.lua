--ʹ�ó����ҩ3200635ȡ����������9425801
--�ɷ�ʹ�ú���
function i3200635_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
        --��������壬����ʹ��
	if MapID ~= 3084408527 then
	    bRet = 43
	end

	--����
	return bRet, bIgnore
end

--ʹ��Ч��
function i3200635_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       unit.CancelBuff(MapID, InstanceID, TargetID, 9425801)
end

--ע��
aux.RegisterItemEvent(3200635, 1, "i3200635_QuestUsable")
aux.RegisterItemEvent(3200635, 0, "i3200635_CanUse")

