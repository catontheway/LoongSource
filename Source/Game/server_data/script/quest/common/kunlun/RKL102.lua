--RKL102
--����1��ʹ�������ڵ�2615028�������15102

--ʹ��Ч��
function i2615028_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       role.AddQuest(TargetID, 15102)
end

--ע��
aux.RegisterItemEvent(2615028, 1, "i2615028_QuestUsable")
