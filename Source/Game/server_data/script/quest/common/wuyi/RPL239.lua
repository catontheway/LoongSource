--RPL239
--����1��ʹ���鱨�ϲ�2614247�������15239
--ʹ��Ч��
function i2614247_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       role.AddQuest(TargetID, 14539)
end

--ע��
aux.RegisterItemEvent(2614247, 1, "i2614247_QuestUsable")
