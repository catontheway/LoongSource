--ʹ�÷�Ѫ��ҩ3200559ȡ����Ѫ9337301
--�ɷ�ʹ�ú���
function i3200559_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
        --������������ʹ��
	if MapID ~= 3000521935 then
	    bRet = 43
	end

	--����
	return bRet, bIgnore
end

--ʹ��Ч��
function i3200559_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       unit.CancelBuff(MapID, InstanceID, TargetID, 9337301)
end

--ע��
aux.RegisterItemEvent(3200559, 1, "i3200559_QuestUsable")
aux.RegisterItemEvent(3200559, 0, "i3200559_CanUse")

