--ʹ�ü�����ҩ3200560ȡ���������9337401
--�ɷ�ʹ�ú���
function i3200560_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
        --��������֮�ȣ�����ʹ��
	if MapID ~= 3000523727 then
	    bRet = 43
	end

	--����
	return bRet, bIgnore
end

--ʹ��Ч��
function i3200560_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       unit.CancelBuff(MapID, InstanceID, TargetID, 9337401)
end

--ע��
aux.RegisterItemEvent(3200560, 1, "i3200560_QuestUsable")
aux.RegisterItemEvent(3200560, 0, "i3200560_CanUse")

