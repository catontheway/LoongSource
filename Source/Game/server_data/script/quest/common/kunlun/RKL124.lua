--RKL124
--����1��ʹ��С���ܺ�2615036�������15124
--ʹ��Ч��
function i2615036_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       role.AddQuest(TargetID, 15124)
end

--ע��
aux.RegisterItemEvent(2615036, 1, "i2615036_QuestUsable")
