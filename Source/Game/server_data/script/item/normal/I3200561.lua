--ʹ��������ҩ3200561ȡ��ɢ����9337601

--�ɷ�ʹ�ú���
function i3200561_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false

	--�������ħ�������ʹ��
	if MapID ~= 3000523471 then
	    bRet = 43
	end

	--����
	return bRet, bIgnore
end

--ʹ��Ч��
function i3200561_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       unit.CancelBuff(MapID, InstanceID, TargetID, 9337601)
end

--ע��
aux.RegisterItemEvent(3200561, 1, "i3200561_QuestUsable")
aux.RegisterItemEvent(3200561, 0, "i3200561_CanUse")

