--RWS108 ʹ�ðټҷ���2613625������BUFF���ڷ��ڣ�2018901�������루2613626���ͽ�ȡ����13109

--�ɷ�ʹ�ú���
function i2613625_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	--���������ˮ������ʹ��
	if MapID ~= 3017299919 then
	    bRet = 43
	end
    return bRet, bIgnore
end

--ʹ��Ч��
function i2613625_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       role.QuestAddRoleItem(MapID, InstanceID, TargetID, 13108, 2613626, 1, 0, 102)
       unit.AddBuff(MapID, InstanceID, TargetID, 2018901, TargetID)
       role.AddQuest(TargetID, 13109)
end

--ע��
aux.RegisterItemEvent(2613625, 1, "i2613625_QuestUsable")
aux.RegisterItemEvent(2613625, 0, "i2613625_CanUse")
