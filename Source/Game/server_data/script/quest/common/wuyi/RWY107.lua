--���RWY107����BUFF��Խ����ף����2019201��

--�������ʱ
function q14107_OnComplete(MapID, InstanceID, questID, ownerID, enderID)
	unit.AddBuff(MapID, InstanceID, ownerID, 2019201, ownerID)
end

--ע��
aux.RegisterQuestEvent(14107, 1, "q14107_OnComplete")

