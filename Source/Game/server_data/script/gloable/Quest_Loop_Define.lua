--����������Զ���

QuestTargetType = {}
QuestTargetType["Kill"]    = 1		--��ɱĳ�ֹ���
QuestTargetType["Collect"] = 2		--�ռ���Ʒ
QuestTargetType["NPCTalk"] = 3		--NPC�Ի�


--�õ��������table��һ�������������ԣ�tablename ��������Num ����������ֵ��
function GetRandQuestTable(tablename, Index)
	--��ɱ�������͵�����
	if tablename[Index].TargetType == QuestTargetType["Kill"] then
		--���ع���ID������
		if tablename[Index].MsgID then
		    return QuestTargetType["Kill"], tablename[Index].MsgID, tablename[Index].TargetID1, tablename[Index].Num1, tablename[Index].TargetID2, tablename[Index].Num2, tablename[Index].TargetID3, tablename[Index].Num3, tablename[Index].TargetID4, tablename[Index].Num4
		else
		    return QuestTargetType["Kill"], 0, tablename[Index].TargetID1, tablename[Index].Num1, tablename[Index].TargetID2, tablename[Index].Num2, tablename[Index].TargetID3, tablename[Index].Num3, tablename[Index].TargetID4, tablename[Index].Num4
		end
	end

	--�ռ���Ʒ���͵�����
	if tablename[Index].TargetType == QuestTargetType["Collect"] then
		--������ƷTypeID��������
		if tablename[Index].MsgID then
		    return QuestTargetType["Collect"], tablename[Index].MsgID, tablename[Index].TargetID1, tablename[Index].Num1, tablename[Index].TargetID2, tablename[Index].Num2, tablename[Index].TargetID3, tablename[Index].Num3, tablename[Index].TargetID4, tablename[Index].Num4
		else
		    return QuestTargetType["Collect"], 0, tablename[Index].TargetID1, tablename[Index].Num1, tablename[Index].TargetID2, tablename[Index].Num2, tablename[Index].TargetID3, tablename[Index].Num3, tablename[Index].TargetID4, tablename[Index].Num4    
		end
	end

	--NPC�Ի����͵�����
	if tablename[Index].TargetType == QuestTargetType["NPCTalk"] then
		--����NPCTypeID
		if tablename[Index].MsgID then
		    return QuestTargetType["NPCTalk"], tablename[Index].MsgID, tablename[Index].TargetID1, tablename[Index].TargetID2, tablename[Index].TargetID3, tablename[Index].TargetID4
	    else
	        return QuestTargetType["NPCTalk"], 0, tablename[Index].TargetID1, tablename[Index].TargetID2, tablename[Index].TargetID3, tablename[Index].TargetID4 
	    end
    end
end

