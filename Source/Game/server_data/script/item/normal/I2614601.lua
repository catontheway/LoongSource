--������
--�ؾ����˵��䣬ʰȡΨһ��ʹ�ú�����Ӿ����BUFF2016601

function I2614601_QuestUsable(MapID, InstanceID, TypeID, TargetID)

	unit.AddBuff(MapID, InstanceID, TargetID, 2016601, TargetID)
end

aux.RegisterItemEvent(2614601, 1, "I2614601_QuestUsable")
