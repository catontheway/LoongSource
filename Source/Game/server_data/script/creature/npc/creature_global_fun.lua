--�������ȫ�ֺ�������
--�������������(3011065)     1		���Լ�¼ˢ��������ҵ�ID


--ͬ����Χ������
function g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, EnmityValue, Radius, High)
    --��ȡ������Χ����������ID
    local AroundCreatureID = {}
    AroundCreatureID = cre.GetAroundCreature(MapID, InstanceID, CreatureID, Radius, High)
    for n in pairs(AroundCreatureID) do
        --�õ������Ŀ���໥��ĵ��ҹ�ϵ
        local Relation = unit.FriendEnemy(MapID, InstanceID, TargetID, AroundCreatureID[n])
        --�жϹ����Ŀ��Ϊ�з�
        if Relation and Relation == 2 then
            --��ӳ��
            cre.AddEnmity(MapID, InstanceID, AroundCreatureID[n], TargetID, EnmityValue)
        end    
    end    
end

--[[-- Ӧ��ʵ��(����ս��ʱ������
function c1008121_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(1008121, 2, "c1008121_OnEnterCombat")]]
























